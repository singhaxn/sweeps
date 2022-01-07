//	FObject.cpp

class Object {
	private:
		char *name;
		struct SweepListNode {
			Sweep *sweep;
			SweepListNode *next;
		} *head, *tail;
		
		struct SweepListNode *IteratorPtr;
		friend class ObjectFileParser;
	public:
		Object()
		{
			IteratorPtr=head=tail=NULL;
			name=new char[10];
			strcpy(name, "object2");
		}
		
		void addSweep(Sweep *s)
		{
			SweepListNode *temp=new SweepListNode();
			
			temp->sweep=s;
			temp->next=NULL;
			if(tail==NULL)
				head=temp;
			else
				tail->next=temp;
			tail=temp;
			cerr<<(void*)s<<endl;
			name=new char[10];
			strcpy(name, "object2");
		}
		
		void deleteSweep()
		{
		}
		
		bool MeshPoints(XYZ p[4],bool Reset=NO) {
			struct SweepListNode *temp=head;
			while(temp!=NULL)
			{
				glColor3f(temp->sweep->color.r,temp->sweep->color.g,
					temp->sweep->color.b);
				while((temp->sweep)->MeshPoints(p)) return 1;
				temp=temp->next;
			}
			return 0;
		}
		
		bool Iterator(Sweep **Out,bool Reset = 0){
			
			if(Reset)
			{
				printf("\nIterator Reset\n");
				//*Out = NULL;
				IteratorPtr = head;
				return 0;
			}
			printf("\nIterator Called ..");
			if(IteratorPtr == NULL){
				printf("End Of List\n");
				IteratorPtr = head;
				return 0;
			}
			printf(" returning Sweep\n");
			*Out = IteratorPtr->sweep;
			IteratorPtr = IteratorPtr->next;
			return 1;	
		}
		
		void saveAs(char *filename);
		void writeXML(ofstream &fout, int indent);
};
