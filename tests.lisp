;; Program for generating test cases for the priority queue

;; (key . priority) pairs

(defun contains? (alist key)
  (loop for kp in alist
	  thereis (equal key (car kp))))

;; The 4 main list operations
(defun insert-sorted (sorted-list key priority)
  "Inserts (key . priority) into an alist sorted by priority."
  (do ((front nil (cons (car back) front))
       (back sorted-list (cdr back)))
      ((or (not back)
	   (> (cdar back) priority))
       (append (reverse front)
	       (list (cons key priority))
	       back))))
;; (insert-sorted '((a . 1) (b . 3)) 'c 2)
;; (insert-sorted '((a . 1) (b . 3)) 'c 0)

(defun delete-sorted (sorted-list key)
  (remove-if (lambda (p)
	       (equal (car p) key))
	     sorted-list))

(defun read-sorted (sorted-list key)
  (cdr (assoc key sorted-list :test #'equal)))

(defun update-sorted (sorted-list key new-priority)
  (let ((lst (delete-sorted sorted-list key)))
    (insert-sorted lst key new-priority)))
;; (update-sorted '((a . 1) (b . 3) (c . 4)) 'b 10)

;; Generate random actions
(defun random-choice (lst)
  (elt lst (random (length lst))))

(defun random-key ()
  (random 10))

(defun random-priority ()
  (random 20))

(defun print-sorted-list (lst &optional (stream t))
  "Prints the state of the sorted list to stream."
  (format stream "~A ~{~A ~}"
	  (length lst)
	  (append (mapcar #'car lst)
		  (mapcar #'cdr lst))))
;; (print-sorted-list '((a . 1) (b . 2)))

(defun do-action (sorted-list action key priority)
  "Returns the sorted list after the operation
and whether it should raise an exception."
  (let ((found (contains? sorted-list key)))
    (case action
      (new (values nil nil))
      (insert (if found
		  (values sorted-list t)
		  (values (insert-sorted sorted-list key priority) nil)))
      (erase (if found
		 (values (delete-sorted sorted-list key) nil)
		 (values sorted-list t)))
      (read (if found
		(values (read-sorted sorted-list key) nil)
		(values -123456 t))) ; some dummy value
      (update (if found
		  (values (update-sorted sorted-list key priority) nil)
		  (values sorted-list t))))))

(defun do-random-actions (trials &optional (output-stream t))
  "Do a number of trials with randomly-chosen actions, keys, and priorities.
Writes the state after each action to output-stream."
  (labels ((inner-loop (i sorted-list)
	     (when (< i trials)
	       ;; Choose random actions
	       (let ((action (random-choice '(new insert erase read update)))
		     (key (random-key))
		     (priority (random-priority)))
		 (multiple-value-bind (result throw-exception)
		     (do-action sorted-list action key priority)
		   ;; Output results to stream
		   (format output-stream
			   "~A ~A ~A ~A ~A~%"
			   action key priority
			   throw-exception
			   (if (listp result)
			       (print-sorted-list result nil)
			       result))
		   ;; Repeat
		   (inner-loop (+ i 1) (if (listp result)
					   result
					   sorted-list)))))))
    (inner-loop 0 nil)))

;; Generate test cases and save to file
(with-open-file (stream "test-cases.txt"
			:direction :output
			:if-exists :supersede)
  (do-random-actions 100000 stream))
