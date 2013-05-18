#ifndef _DEF_WALL
#define _DEF_WALL

class Wall : public AObject
{
private:
Wall();
Wall(Wall const &)
Wall &operator=(Wall const &)
public :
virtual ~Wall();
virtual void initialize(void);
};


#endif // !_DEF_AOBJECT
