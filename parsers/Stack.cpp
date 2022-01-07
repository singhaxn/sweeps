//	Stack Template

#ifndef _STACK
#define _STACK
#define STACK_SIZE 50

template <class T>
class Stack
{
	private:
		T *value;
		int top;
		
	public:
		Stack()
		{
			value=new T[STACK_SIZE];
			top=-1;
		}
		
		//	Initialize Stack to val
		Stack(T val)
		{
			value=new T[STACK_SIZE];
			top=-1;
			for(int i=0; i<STACK_SIZE; i++)
				value[i]=val;
		}
		
		void push(T val)
		{
			value[++top]=val;
		}
		
		T pop()
		{
			T temp;
			
			temp=value[top];
			top--;
			return temp;
		}
		
		//	Return topmost value from stack
		T peek()
		{
			return value[top];
		}
		
		int count()
		{
			return top+1;
		}
		
		//	Initialize Stack to val if stack is empty
		void init(T val)
		{
			if (top==-1)
				for(int i=0; i<STACK_SIZE; i++)
					value[i]=val;
		}
		
		void clear()
		{
			top=-1;
		}
		
		~Stack()
		{
			delete [] value;
		}
};

#endif
