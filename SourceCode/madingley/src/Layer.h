#ifndef LAYER
#define	LAYER

class Layer {
public:

    virtual ~Layer( ) {
        ;
    }
    virtual double& operator[]( int ) = 0;
    virtual void SetTime( unsigned ) = 0;
};

#endif

