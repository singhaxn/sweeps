//	XMLHandling.cpp

#ifndef _XML_HANDLING
#define _XML_HANDLING

#include <expat.h>
#include <string.h>
#include <fstream>
#include <strstream>

#define XML_PARSER_BUF_LEN 1024

//	@@	Place the following functions in appropriate classes
void Object::saveAs(char *filename)
{
	ofstream fout(filename);
	
	fout<<"<?xml version=\"1.0\"?>\n";
	fout<<"<!DOCTYPE object>\n\n";
	writeXML(fout, 0);
	
	fout.close();
}

void Object::writeXML(ofstream &fout, int indent)
{
	cerr<<"\nXML_Object...\n";
	fout<<"<object name=\""<<name<<"\">\n";
	for(SweepListNode *stemp=head; stemp!=NULL; stemp=stemp->next)
		stemp->sweep->writeXML(fout, indent+1);
	fout<<"</object>\n";
}

void Sweep::saveAs(char *filename)
{
	ofstream fout(filename);
	
	fout<<"<?xml version=\"1.0\"?>\n";
	fout<<"<!DOCTYPE sweep>\n\n";
	writeXML(fout, 0);
	
	fout.close();
}

void Sweep::writeXML(ofstream &fout, int indent)
{
	cerr<<"\nXML_Sweep...\n";
	char indentstr[indent+1];
	
	memset(indentstr, '\t', indent);
	indentstr[indent]='\0';
	
	fout<<indentstr<<"<sweep name=\""<<name<<"\" rotate=\""<<rotation<<"\">\n";
	fout<<indentstr<<"\t<path>\n";
	Path->writeXML(fout, indent+2);
	fout<<indentstr<<"\t</path>\n";
	
	fout<<indentstr<<"\t<curvelist>\n";
/*	for(Curve *ctemp=Surface; ctemp!=NULL; ctemp=ctemp->next)
		ctemp->writeXML(fout, indent+2);*/
	Surface->writeXML(fout, indent+2);
	fout<<indentstr<<"\t</curvelist>\n";
	if(s!=NULL) {
		fout<<indentstr<<"\t<fx>"<<f[0]<<"</fx>\n";
		fout<<indentstr<<"\t<fy>"<<f[1]<<"</fy>\n";
		fout<<indentstr<<"\t<fz>"<<f[2]<<"</fz>\n";
		fout<<indentstr<<"\t<s>"<<s<<"</s>\n";
	}
	fout<<indentstr<<"</sweep>\n";
}

/*void Sweep::setProperties(const char **attr)
{
	for(int i=0; attr[i]; i++) {
		if(!strcmp(attr[i], "name"))
			name=strdup(attr[i+1]);
		else if(!strcmp(attr[i], "rotate"))
			rotation=strcmp(attr[i+1], "false");
	}
}*/

void Curve::saveAs(char *filename)
{
	ofstream fout(filename);
	
	fout<<"<?xml version=\"1.0\"?>\n";
	fout<<"<!DOCTYPE curve>\n\n";
	writeXML(fout, 0);
	
	fout.close();
}

void Parametric::writeXML(ofstream &fout, int indent)
{
	cerr<<"\nXML_Parametric...\n";
	char indentstr[indent+1];
	
	memset(indentstr, '\t', indent);
	indentstr[indent]='\0';
	
	fout<<indentstr<<"<curve type=\"parametric\" name=\""<<name<<"\" npoints=\""
		<<nPoints<<"\" tmin=\""<<tmin<<"\" tmax=\""<<tmax<<"\">\n";
	fout<<indentstr<<'\t'<<"<x>"<<sf[0]<<"</x>\n";
	fout<<indentstr<<'\t'<<"<y>"<<sf[1]<<"</y>\n";
	fout<<indentstr<<'\t'<<"<z>"<<sf[2]<<"</z>\n";
	fout<<indentstr<<"</curve>\n";
}

void Parametric::setProperties(const char **attr)
{
	for(int i=0; attr[i]!=NULL; i+=2) {
		if(!strcmp(attr[i], "name"))
			name=strdup(attr[i+1]);
		if(!strcmp(attr[i], "npoints"))
			nPoints=atoi(attr[i+1]);
		if(!strcmp(attr[i], "tmin"))
			tmin=strtod(attr[i+1], NULL);
		if(!strcmp(attr[i], "tmax"))
			tmax=strtod(attr[i+1], NULL);
	}
}

void Parametric::setExtProperty(int index, const char *buffer)
{
	if(index<0 || index>2)
		return;
	sf[index]=strdup(buffer);
	ef[index].parse(sf[index]);
}

void BazierCurve::writeXML(ofstream &fout, int indent)
{
	cerr<<"\nXML_Bezier...\n";
	char indentstr[indent+1];
	char point[64];
	
	memset(indentstr, '\t', indent);
	indentstr[indent]='\0';
	
	fout<<indentstr<<"<curve type=\"bezier\" name=\""<<name<<"\" npoints=\""<<nPoints<<"\">\n";
	fout<<indentstr<<'\t'<<"<start>"<<cntrlPoints[0].toString(point)<<"</start>\n";
	fout<<indentstr<<'\t'<<"<ctrl1>"<<cntrlPoints[1].toString(point)<<"</ctrl1>\n";
	fout<<indentstr<<'\t'<<"<ctrl2>"<<cntrlPoints[2].toString(point)<<"</ctrl2>\n";
	fout<<indentstr<<'\t'<<"<end>"<<cntrlPoints[3].toString(point)<<"</end>\n";
	fout<<indentstr<<"</curve>\n";
}

void BazierCurve::setProperties(const char **attr)
{
	for(int i=0; attr[i]!=NULL; i+=2) {
		if(!strcmp(attr[i], "name"))
			name=strdup(attr[i+1]);
		if(!strcmp(attr[i], "npoints"))
			nPoints=atoi(attr[i+1]);
	}
}

void BazierCurve::setExtProperty(int index, const char *buffer)
{
	if(index<0 || index>3)
		return;
	cntrlPoints[index].parsePoint(buffer);
}

void Lagranges::writeXML(ofstream &fout, int indent)
{
	cerr<<"\nXML_Lagranges...\n";
	char indentstr[indent+1];
	char point[64];
	
	memset(indentstr, '\t', indent);
	indentstr[indent]='\0';
	
	fout<<indentstr<<"<curve type=\"lagrange\" name=\""<<name<<"\" npoints=\""<<nPoints<<"\" lagpoints=\""<<nCtrlPoints<<"\">\n";
//	for(XYZ *pt=first, pt!=NULL; pt=pt->next)
	for(int i=0; i<nCtrlPoints; i++)
		fout<<indentstr<<'\t'<<"<point>"<<cntrlPoints[i].toString(point)<<"</point>\n";
	fout<<indentstr<<"</curve>\n";
}

void Lagranges::setProperties(const char **attr)
{
	for(int i=0; attr[i]!=NULL; i+=2) {
		if(!strcmp(attr[i], "name"))
			name=strdup(attr[i+1]);
		if(!strcmp(attr[i], "npoints"))
			nPoints=atoi(attr[i+1]);
		if(!strcmp(attr[i], "lagpoints")) {
			nCtrlPoints=atoi(attr[i+1]);
			cntrlPoints=new XYZ[nCtrlPoints];
		}
	}
}

void Lagranges::setExtProperty(int index, const char *buffer)
{
	static int cnt=0;
	
	cntrlPoints[cnt++].parsePoint(buffer);
	
	if(cnt==nCtrlPoints)
		cnt=0;
}

char *XYZ::toString(char *res) const
{
	sprintf(res, "(%lf, %lf, %lf)", x, y, z);
	return res;
}

void XYZ::parsePoint(const char *point)
{
	sscanf(point, "(%lf, %lf, %lf)", &x, &y, &z);
}

void Sweep::setFunction(int index, char *fun)
{
	if(index<0 || index>2)
		return;
	
	f[index]=strdup(fun);
	ef[index].parse(f[index]);
}

void Sweep::setParametricFunction(char *buffer)
{
	s=strdup(buffer);
	es.parse(s);
}

class XMLFileParser {
	protected:
		static bool parsing;
		static unsigned int pos;
		static char buffer[XML_PARSER_BUF_LEN];
	
//	public:
//		virtual void parse(char *filename)=0;
};

bool XMLFileParser::parsing=0;
unsigned int XMLFileParser::pos=0;
char XMLFileParser::buffer[XML_PARSER_BUF_LEN]={0};

class ObjectFileParser : public XMLFileParser {
	private:
	/*
		Contents of Object class:
		
		Sweep *head;
		char *name;
		Position *pos;
		Sweep *last;	@@	Remember to add this
		
		@@	Remember to make this class a friend of Object class
	*/
		enum ElementTypes {
			OBJECT, SWEEP, PATH, CURVELIST, CURVE, X, Y, Z, START, CTRL1,
			CTRL2, END, POINT, FX, FY, FZ, T, ROTATION, TRANSLATION, UNKNOWN
		};
		
		struct SCurve {
			int type;		//	0=para, 1=bez
			int nPoints;
			char *name;
			
			union {
				struct {
					//	Para
					char *sp[3];
					double tmin;
					double tmax;
				} para;
			
				struct {
					//	Bez
					XYZ cntrlPointsIN[4];
				} bez;
			} curve;
			
			SCurve() {
				memset(this, '\0', sizeof(SCurve));
			}
		};
		
		struct SSweep {
			Curve *SurfaceIN, *SILast;
			Curve *PathIN;
			char *fun[3];
			char *sp;
			double tmin;
			double tmax;
			bool rot;
			char *name;
		};
		
		static Object *object;
		static SSweep sweep;
		static SCurve curve;
		static ElementTypes xpath[6];
		static int xpathpos;
		
		static ElementTypes elementType(const char *name)
		{
			switch(name[0]) {
				case 'c':
					if(!strcmp(name, "curvelist"))
						return CURVELIST;
					if(!strcmp(name, "curve"))
						return CURVE;
					if(!strcmp(name, "ctrl1"))
						return CTRL1;
					if(!strcmp(name, "ctrl2"))
						return CTRL2;
					break;
				
				case 'e':
					if(!strcmp(name, "end"))
						return END;
					break;
				
				case 'f':
					if(!strcmp(name, "fx"))
						return FX;
					if(!strcmp(name, "fy"))
						return FY;
					if(!strcmp(name, "fz"))
						return FZ;
					break;
				
				case 'o':
					if(!strcmp(name, "object"))
						return OBJECT;
					break;
				
				case 'p':
					if(!strcmp(name, "point"))
						return POINT;
					if(!strcmp(name, "path"))
						return PATH;
					break;
				
				case 'r':
					if(!strcmp(name, "rotation"))
						return ROTATION;
					break;
				
				case 's':
					if(!strcmp(name, "s"))
						return T;
					if(!strcmp(name, "sweep"))
						return SWEEP;
					if(!strcmp(name, "start"))
						return START;
					break;
				
				case 't':
					if(!strcmp(name, "translation"))
						return TRANSLATION;
					break;
				
				case 'x':
					if(!strcmp(name, "x"))
						return X;
					break;
				
				case 'y':
					if(!strcmp(name, "y"))
						return Y;
					break;
				
				case 'z':
					if(!strcmp(name, "z"))
						return Z;
					break;
			}
			
			return UNKNOWN;
		}
		
		static void XMLCALL startHandler(void *userData, const char *name, const char **attr)
		{
			ElementTypes etype=elementType(name);
			parsing=1;
			
			switch(etype) {
				case OBJECT:
					if(!strcmp(attr[0], "name"))
						object->name=strdup(attr[1]);
					xpathpos=0;
					
					break;
				case SWEEP:
					memset(&sweep, '\0', sizeof(SSweep));
					for(int i=0; attr[i]; i+=2) {
						if(!strcmp(attr[i], "name"))
							sweep.name=strdup(attr[i+1]);
						if(!strcmp(attr[i], "rotate"))
							sweep.rot=strcmp(attr[i+1],"0");
					}
					
					break;
				
				case CURVE:
				{
					memset(&curve, '\0', sizeof(SCurve));
					for(int i=0; attr[i]; i+=2)
						if(!strcmp(attr[i], "type")) {
							//	@@	Remember to add default constructors for derivatives of Curve
							if(!strcmp(attr[i+1], "parametric"))
								curve.type=0;
							else if(!strcmp(attr[i+1], "bezier"))
								curve.type=1;
							break;
						}
					for(int i=0; attr[i]; i+=2) {
						if(!strcmp(attr[i], "name"))
							curve.name=strdup(attr[i+1]);
						else if(!strcmp(attr[i], "npoints"))
							curve.nPoints=atoi(attr[i+1]);
						else if(curve.type==0) {
							if(!strcmp(attr[i], "tmin"))
								curve.curve.para.tmin=strtod(attr[i+1], NULL);
							if(!strcmp(attr[i], "tmax"))
								curve.curve.para.tmax=strtod(attr[i+1], NULL);
						}
					}
					break;
				}
				case UNKNOWN:
					break;
			}
			
			xpath[xpathpos++]=etype;
			pos=0;
		}
		
		static void XMLCALL endHandler(void *userData, const XML_Char *name)
		{
			buffer[pos]='\0';
			
			switch(elementType(name)) {
				case SWEEP:
				{
					Sweep *curr;
					
					if(sweep.sp==NULL)
						curr=new Sweep(sweep.SurfaceIN, sweep.PathIN, sweep.fun, sweep.sp, sweep.tmin, sweep.tmax, sweep.rot);
					else
						curr=new Sweep(sweep.SurfaceIN, sweep.PathIN, sweep.rot);
					curr->name=sweep.name;
					object->addSweep(curr);
				}
				case CURVE:
				{
 					Curve *curr;
					
					if(curve.type==0)
						curr=new Parametric(curve.curve.para.sp, curve.curve.para.tmin, curve.curve.para.tmax, curve.nPoints);
					else
						curr=new BazierCurve(curve.curve.bez.cntrlPointsIN, 4, curve.nPoints);
					
					if(xpath[xpathpos-1]==CURVELIST) {
						if(sweep.SurfaceIN==NULL)
							sweep.SurfaceIN=curr;
						else
							sweep.SILast->next=curr;
						sweep.SILast=curr;
					} else
						sweep.PathIN=curr;
					curr->name=curve.name;
					break;
				}
				
				case X:
					curve.curve.para.sp[0]=strdup(buffer);
					break;
				case Y:
					curve.curve.para.sp[1]=strdup(buffer);
					break;
				case Z:
					curve.curve.para.sp[2]=strdup(buffer);
					break;
				
				//	@@	Remember to add setExtProperty(int, char*); for class Bezier
				case START:
					//	@@	Remember to add parseString(char*); for class XYZ
					curve.curve.bez.cntrlPointsIN[0].parsePoint(buffer);
					break;
				case CTRL1:
					curve.curve.bez.cntrlPointsIN[1].parsePoint(buffer);
					break;
				case CTRL2:
					curve.curve.bez.cntrlPointsIN[2].parsePoint(buffer);
					break;
				case END:
					curve.curve.bez.cntrlPointsIN[3].parsePoint(buffer);
					break;
				
				//	@@	Remember to add setExtProperty(int, char*); for class Lagrange
/*				case POINT:
//					object->tail->getLast()->setExtProperty(0, buffer);
					object->tail->sweep->Surface->setExtProperty(0, buffer);
					break;*/
				
				case FX:
					sweep.fun[0]=strdup(buffer);
					break;
				case FY:
					sweep.fun[1]=strdup(buffer);
					break;
				case FZ:
					sweep.fun[2]=strdup(buffer);
					break;
				case T:
					sweep.sp=strdup(buffer);
					break;
				
				//	If possible add these fields to the Scene class
/*				case ROTATION:
					object->tail->setRotation(buffer);
					break;
				case TRANSLATION:
					object->tail->setTranslation(buffer);
					break;*/
				
				case UNKNOWN:
					break;
			}
			
			xpathpos--;
			pos=0;
		}
		
		static void XMLCALL charHandler(void *userData, const XML_Char *s, int len)
		{
			int i;
			char temp[len+1];
			
			memcpy(buffer+pos, s, len);
			pos+=len;
		}
		
	public:
		ObjectFileParser(Object *obj)
		{
			object=obj;
		}
		
		void setObject(Object *obj)
		{
			object=obj;
		}
		
		void parse(char *filename)
		{
			XML_Parser parser=XML_ParserCreate(NULL);
			FILE *fin=fopen(filename, "r");
			int len;
			
			XML_SetElementHandler(parser, ObjectFileParser::startHandler, ObjectFileParser::endHandler);
			XML_SetCharacterDataHandler(parser, ObjectFileParser::charHandler);
			
			parsing=0;
			xpathpos=0;
			do {
				len=fread(buffer, 1, XML_PARSER_BUF_LEN, fin);
				if(len==XML_PARSER_BUF_LEN) {
					if(XML_Parse(parser, buffer, len, 0)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
				} else
					if(XML_Parse(parser, buffer, len, 1)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
			} while(len==XML_PARSER_BUF_LEN);
			
			fclose(fin);
			XML_ParserFree(parser);
		}
};

Object *ObjectFileParser::object=NULL;
ObjectFileParser::ElementTypes ObjectFileParser::xpath[6]={ObjectFileParser::OBJECT};
int ObjectFileParser::xpathpos=0;
ObjectFileParser::SSweep ObjectFileParser::sweep={NULL, NULL, NULL, {NULL, NULL, NULL}, NULL, 0, 0, 0};
ObjectFileParser::SCurve ObjectFileParser::curve;

class SweepFileParser : public XMLFileParser {	
	private:
		enum ElementTypes {
			SWEEP, PATH, CURVELIST, CURVE, X, Y, Z, START, CTRL1,
			CTRL2, END, POINT, FX, FY, FZ, T, ROTATION, TRANSLATION, UNKNOWN
		};

		struct SCurve {
			int type;		//	0=para, 1=bez
			int nPoints;
			char *name;
			
			union {
				struct {
					//	Para
					char *sp[3];
					double tmin;
					double tmax;
				} para;
			
				struct {
					//	Bez
					XYZ cntrlPointsIN[4];
				} bez;
			} curve;
			
			SCurve() {
				memset(this, '\0', sizeof(SCurve));
			}
		};
		
		struct SSweep {
			Curve *SurfaceIN, *SILast;
			Curve *PathIN;
			char *fun[3];
			char *sp;
			double tmin;
			double tmax;
			bool rot;
			char *name;
		};
		
		static SSweep sweep;
		static SCurve curve;

		static ElementTypes xpath[6];
		static int xpathpos;
		static Sweep *retVal;
		
		struct SweepNames {
			char *name;
			SweepNames *next;
		};
		static SweepNames *head, *tail;
		static ElementTypes elementType(const char *name)
		{
			switch(name[0]) {
				case 'c':
					if(!strcmp(name, "curvelist"))
						return CURVELIST;
					if(!strcmp(name, "curve"))
						return CURVE;
					if(!strcmp(name, "ctrl1"))
						return CTRL1;
					if(!strcmp(name, "ctrl2"))
						return CTRL2;
					break;
				
				case 'e':
					if(!strcmp(name, "end"))
						return END;
					break;
				
				case 'f':
					if(!strcmp(name, "fx"))
						return FX;
					if(!strcmp(name, "fy"))
						return FY;
					if(!strcmp(name, "fz"))
						return FZ;
					break;
				
				case 'p':
					if(!strcmp(name, "point"))
						return POINT;
					if(!strcmp(name, "path"))
						return PATH;
					break;
				
				case 'r':
					if(!strcmp(name, "rotation"))
						return ROTATION;
					break;
				
				case 's':
					if(!strcmp(name, "s"))
						return T;
					if(!strcmp(name, "sweep"))
						return SWEEP;
					if(!strcmp(name, "start"))
						return START;
					break;
				
				case 't':
					if(!strcmp(name, "translation"))
						return TRANSLATION;
					break;
				
				case 'x':
					if(!strcmp(name, "x"))
						return X;
					break;
				
				case 'y':
					if(!strcmp(name, "y"))
						return Y;
					break;
				
				case 'z':
					if(!strcmp(name, "z"))
						return Z;
					break;
			}
			
			return UNKNOWN;
		}
		
		static void XMLCALL startHandler(void *userData, const char *name, const XML_Char **attr)
		{
			ElementTypes etype=elementType(name);
			parsing=1;
			
			switch(etype) {
				case SWEEP:
					xpathpos=0;
					memset(&sweep, '\0', sizeof(SSweep));
					for(int i=0; attr[i]; i+=2) {
						if(!strcmp(attr[i], "name"))
							sweep.name=strdup(attr[i+1]);
						if(!strcmp(attr[i], "rotate"))
							sweep.rot=strcmp(attr[i+1],"0");
					}
					
					break;
				
				case CURVE:
				{
					memset(&curve, '\0', sizeof(SCurve));
					for(int i=0; attr[i]; i+=2)
						if(!strcmp(attr[i], "type")) {
							//	@@	Remember to add default constructors for derivatives of Curve
							if(!strcmp(attr[i+1], "parametric"))
								curve.type=0;
							else if(!strcmp(attr[i+1], "bezier"))
								curve.type=1;
							break;
						}
					for(int i=0; attr[i]; i+=2) {
						if(!strcmp(attr[i], "name"))
							curve.name=strdup(attr[i+1]);
						else if(!strcmp(attr[i], "npoints"))
							curve.nPoints=atoi(attr[i+1]);
						else if(curve.type==0) {
							if(!strcmp(attr[i], "tmin"))
								curve.curve.para.tmin=strtod(attr[i+1], NULL);
							if(!strcmp(attr[i], "tmax"))
								curve.curve.para.tmax=strtod(attr[i+1], NULL);
						}
					}
					break;
				}
				case UNKNOWN:
					break;
			}
			
			xpath[xpathpos++]=etype;
			pos=0;
		}
		
		static void XMLCALL endHandler(void *userData, const XML_Char *name)
		{
			buffer[pos]='\0';
			
			switch(elementType(name)) {
					//	@@	Remember to add setExtProperty(int, char*); for class Curve (virtual)
					//	@@	Remember to add setExtProperty(int, char*); for class Parametric
				case SWEEP:
				{
					if(sweep.sp==NULL)
						retVal=new Sweep(sweep.SurfaceIN, sweep.PathIN, sweep.fun, sweep.sp, sweep.tmin, sweep.tmax, sweep.rot);
					else
						retVal=new Sweep(sweep.SurfaceIN, sweep.PathIN, sweep.rot);
					retVal->name=sweep.name;
				}
				case CURVE:
				{
 					Curve *curr;
					
					if(curve.type==0)
						curr=new Parametric(curve.curve.para.sp, curve.curve.para.tmin, curve.curve.para.tmax, curve.nPoints);
					else
						curr=new BazierCurve(curve.curve.bez.cntrlPointsIN, 4, curve.nPoints);
					
					if(xpath[xpathpos-1]==CURVELIST) {
						if(sweep.SurfaceIN==NULL)
							sweep.SurfaceIN=curr;
						else
							sweep.SILast->next=curr;
						sweep.SILast=curr;
					} else
						sweep.PathIN=curr;
					curr->name=curve.name;
					break;
				}
				
				case X:
					curve.curve.para.sp[0]=strdup(buffer);
					break;
				case Y:
					curve.curve.para.sp[1]=strdup(buffer);
					break;
				case Z:
					curve.curve.para.sp[2]=strdup(buffer);
					break;
				
				//	@@	Remember to add setExtProperty(int, char*); for class Bezier
				case START:
					//	@@	Remember to add parseString(char*); for class XYZ
					curve.curve.bez.cntrlPointsIN[0].parsePoint(buffer);
					break;
				case CTRL1:
					curve.curve.bez.cntrlPointsIN[1].parsePoint(buffer);
					break;
				case CTRL2:
					curve.curve.bez.cntrlPointsIN[2].parsePoint(buffer);
					break;
				case END:
					curve.curve.bez.cntrlPointsIN[3].parsePoint(buffer);
					break;
				
				//	@@	Remember to add setExtProperty(int, char*); for class Lagrange
/*				case POINT:
//					object->tail->getLast()->setExtProperty(0, buffer);
					object->tail->sweep->Surface->setExtProperty(0, buffer);
					break;*/
				
				case FX:
					sweep.fun[0]=strdup(buffer);
					break;
				case FY:
					sweep.fun[1]=strdup(buffer);
					break;
				case FZ:
					sweep.fun[2]=strdup(buffer);
					break;
				case T:
					sweep.sp=strdup(buffer);
					break;
				
				//	If possible add these fields to the Scene class
/*				case ROTATION:
					object->tail->setRotation(buffer);
					break;
				case TRANSLATION:
					object->tail->setTranslation(buffer);
					break;*/
				
				case UNKNOWN:
					break;
			}
			xpathpos--;
			pos=0;
		}
		
		static void XMLCALL charHandler(void *userData, const XML_Char *s, int len)
		{
			int i;
			char temp[len+1];
			
			memcpy(buffer+pos, s, len);
			pos+=len;
		}
		
	public:
		SweepFileParser() {}
		
		Sweep *getSweep(char *filename)
		{
			if(filename==NULL)
				return NULL;
			
			XML_Parser parser=XML_ParserCreate(NULL);
			FILE *fin=fopen(filename, "r");
			int len;
			
			XML_SetElementHandler(parser, SweepFileParser::startHandler, SweepFileParser::endHandler);
			XML_SetCharacterDataHandler(parser, SweepFileParser::charHandler);
			
			parsing=0;
			xpathpos=0;
			do {
				len=fread(buffer, 1, XML_PARSER_BUF_LEN, fin);
				if(len==XML_PARSER_BUF_LEN) {
					if(XML_Parse(parser, buffer, len, 0)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
				} else
					if(XML_Parse(parser, buffer, len, 1)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
			} while(len==XML_PARSER_BUF_LEN);
			
			fclose(fin);
			XML_ParserFree(parser);
			cerr<<"parse done\n";
			
			return retVal;
		}
};

SweepFileParser::ElementTypes SweepFileParser::xpath[6]={SweepFileParser::SWEEP};
int SweepFileParser::xpathpos=0;
Sweep *SweepFileParser::retVal=NULL;
SweepFileParser::SweepNames *SweepFileParser::head=NULL;
SweepFileParser::SweepNames *SweepFileParser::tail=NULL;
SweepFileParser::SSweep SweepFileParser::sweep={NULL, NULL, NULL, {NULL, NULL, NULL}, NULL, 0, 0, 0};
SweepFileParser::SCurve SweepFileParser::curve;

class CurveFileParser : public XMLFileParser {
	private:
		enum ElementTypes {
			CURVE, X, Y, Z, START, CTRL1, CTRL2, END, POINT, ROTATION, T, 
			TRANSLATION, UNKNOWN
		};

		struct SCurve {
			int type;		//	0=para, 1=bez
			int nPoints;
			char *name;
			
			union {
				struct {
					//	Para
					char *sp[3];
					double tmin;
					double tmax;
				} para;
			
				struct {
					//	Bez
					XYZ cntrlPointsIN[4];
				} bez;
			} curve;
			
			SCurve() {
				memset(this, '\0', sizeof(SCurve));
			}
		};
		
		static SCurve curve;
		static ElementTypes xpath[6];
		static int xpathpos;
		static Curve *retVal;
		
		static ElementTypes elementType(const char *name)
		{
			switch(name[0]) {
				case 'c':
					if(!strcmp(name, "curve"))
						return CURVE;
					if(!strcmp(name, "ctrl1"))
						return CTRL1;
					if(!strcmp(name, "ctrl2"))
						return CTRL2;
					break;
				
				case 'e':
					if(!strcmp(name, "end"))
						return END;
					break;
				
				case 'p':
					if(!strcmp(name, "point"))
						return POINT;
					break;
				
				case 'r':
					if(!strcmp(name, "rotation"))
						return ROTATION;
					break;
				
				case 's':
					if(!strcmp(name, "start"))
						return START;
					break;
				
				case 't':
					if(!strcmp(name, "translation"))
						return TRANSLATION;
					if(!strcmp(name, "t"))
						return T;
					break;
				
				case 'x':
					if(!strcmp(name, "x"))
						return X;
					break;
				
				case 'y':
					if(!strcmp(name, "y"))
						return Y;
					break;
				
				case 'z':
					if(!strcmp(name, "z"))
						return Z;
					break;
			}
			
			return UNKNOWN;
		}
		
		static void XMLCALL startHandler(void *userData, const char *name, const char **attr)
		{
			ElementTypes etype=elementType(name);
			parsing=1;
			
			switch(etype) {
				case CURVE:
					xpathpos=0;
					memset(&curve, '\0', sizeof(SCurve));
					for(int i=0; attr[i]; i+=2)
						if(!strcmp(attr[i], "type")) {
							//	@@	Remember to add default constructors for derivatives of Curve
							if(!strcmp(attr[i+1], "parametric"))
								curve.type=0;
							else if(!strcmp(attr[i+1], "bezier"))
								curve.type=1;
							break;
						}
					for(int i=0; attr[i]; i+=2) {
						if(!strcmp(attr[i], "name"))
							curve.name=strdup(attr[i+1]);
						else if(!strcmp(attr[i], "npoints"))
							curve.nPoints=atoi(attr[i+1]);
						else if(curve.type==0) {
							if(!strcmp(attr[i], "tmin"))
								curve.curve.para.tmin=strtod(attr[i+1], NULL);
							if(!strcmp(attr[i], "tmax"))
								curve.curve.para.tmax=strtod(attr[i+1], NULL);
						}
					}
					break;
				case UNKNOWN:
					break;
			}
			
			xpath[xpathpos++]=etype;
			pos=0;
		}
		
		static void XMLCALL endHandler(void *userData, const XML_Char *name)
		{
			buffer[pos]='\0';

			switch(elementType(name)) {
				//	@@	Remember to add setExtProperty(int, char*); for class Curve (virtual)
				//	@@	Remember to add setExtProperty(int, char*); for class Parametric
				case CURVE:
				{
					Curve *curr;
					
					if(curve.type==0)
						curr=new Parametric(curve.curve.para.sp, curve.curve.para.tmin, curve.curve.para.tmax, curve.nPoints);
					else
						curr=new BazierCurve(curve.curve.bez.cntrlPointsIN, 4, curve.nPoints);
					curr->name=curve.name;
					
					retVal=curr;
					break;
				}
				
				case X:
					curve.curve.para.sp[0]=strdup(buffer);
					break;
				case Y:
					curve.curve.para.sp[1]=strdup(buffer);
					break;
				case Z:
					curve.curve.para.sp[2]=strdup(buffer);
					break;
				
				//	@@	Remember to add setExtProperty(int, char*); for class Bezier
				case START:
					//	@@	Remember to add parseString(char*); for class XYZ
					curve.curve.bez.cntrlPointsIN[0].parsePoint(buffer);
					break;
				case CTRL1:
					curve.curve.bez.cntrlPointsIN[1].parsePoint(buffer);
					break;
				case CTRL2:
					curve.curve.bez.cntrlPointsIN[2].parsePoint(buffer);
					break;
				case END:
					curve.curve.bez.cntrlPointsIN[3].parsePoint(buffer);
					break;
					
					//	@@	Remember to add setExtProperty(int, char*); for class Lagrange
	/*				case POINT:
	//					object->tail->getLast()->setExtProperty(0, buffer);
						object->tail->sweep->Surface->setExtProperty(0, buffer);
						break;*/
					
					//	If possible add these fields to the Scene class
	/*				case ROTATION:
						object->tail->setRotation(buffer);
						break;
					case TRANSLATION:
						object->tail->setTranslation(buffer);
						break;*/
					
				case UNKNOWN:
					break;
			}
			xpathpos--;
			pos=0;
		}
		
		static void XMLCALL charHandler(void *userData, const XML_Char *s, int len)
		{
			int i;
			char temp[len+1];
			
			memcpy(buffer+pos, s, len);
			pos+=len;
		}
		
	public:
		CurveFileParser() {}
		
		Curve *getCurve(char *filename)
		{
			if(filename==NULL)
				return NULL;
			
			XML_Parser parser=XML_ParserCreate(NULL);
			FILE *fin=fopen(filename, "r");
			int len;
			
			XML_SetElementHandler(parser, CurveFileParser::startHandler, CurveFileParser::endHandler);
			XML_SetCharacterDataHandler(parser, CurveFileParser::charHandler);
			
			parsing=0;
			xpathpos=0;
			do {
				len=fread(buffer, 1, XML_PARSER_BUF_LEN, fin);
				if(len==XML_PARSER_BUF_LEN) {
					if(XML_Parse(parser, buffer, len, 0)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
				} else
					if(XML_Parse(parser, buffer, len, 1)==XML_STATUS_ERROR && parsing) {
						printf("ERROR: %s on line %d column %d\n", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser), XML_GetCurrentColumnNumber(parser));
						break;
					}
			} while(len==XML_PARSER_BUF_LEN);
			
			fclose(fin);
			XML_ParserFree(parser);
			
			return retVal;
		}
};

CurveFileParser::ElementTypes CurveFileParser::xpath[6]={CurveFileParser::CURVE};
int CurveFileParser::xpathpos=0;
Curve *CurveFileParser::retVal=NULL;
CurveFileParser::SCurve CurveFileParser::curve;

#endif
