Sweeps.out:FMain.cpp FCurves.cpp FBazier.h FVector.h FObject.cpp
	g++ FMain.cpp -o Sweeps.out -L /usr/X11R6/lib -lglut -lm -lpthread -lexpat -Wno-deprecated
	./Sweeps.out &
bezier:Fbezier.c FBazier.h
	g++ Fbezier.c -o bezier.out -L /usr/X11R6/lib -lglut -lm 

clean:
	rm Sweeps.out
	rm bezier.out

	
