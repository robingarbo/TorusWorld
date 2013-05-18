#ifndef _DEF_AOBJECT
#define _DEF_AOBJECT

class AObject
{
AObject(void);
AObject(int);
virtual ~AObject(void);
virtual void initialize(void);
int	getType(void) const;
protected:
int type;
};


#endif // !_DEF_AOBJECT
