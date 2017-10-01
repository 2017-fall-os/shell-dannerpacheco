Instructions:
	 create executable: $ make
	 run shell: $ ./shell
	 clean: $ make clean


Objective:
	In this lab we had to create a basic shell that executes commands using system calls and our tokenizer from lab 1.

Acknowledgments:
	Jose Cabrera explained the concept of the lab and what we had to do in general. He also explained how execve works.
	Jose Yanez helped me debugg my code and I helped him debugged his code.
	Used fork from Dr. Freudenthal. 


Part 2:

Instructions:
	 create executable: $ make
	 run shell: $ ./shell
	 clean: $ make clean

Objective:
	In the second part, we had to implement the cd command and enable simple pipes and background tasks.

Collaborations:
	Ulises Martinez helped me understand how to enable background tasks.
	I explained to Jose Yanez how to use pipes in a simple way using dup2()
	They both helped me debug my code, and I helped them with theirs, but we did not copy code from each other.


Issues:
      -For some when you click enter again it grabs what was in the buffer again.
      -background tasks only work without pipes
	
