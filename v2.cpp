#include "TXLib.h"

//{//////////////////////////////////////INITIALIZATER LIST//////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class manager_t;

bool Inside (POINT p, double x1, double y1, double x2, double y2);

//}

//{///////////////////////////////////////////CLASSES////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct point_t
    {
    double x;
    double y;

    point_t () :
        x (0),
        y (0)
        {}

    template <typename Tx, typename Ty>
    point_t (const Tx& Ax, const Ty& Ay) :
        x (Ax),
        y (Ay)
        {}

    template <typename T>
    point_t (const T& p) :
        x (p.x),
        y (p.y)
        {}

    operator POINT ()
        {
        return POINT {x, y};
        }
    };

//

class base_t
    {
    public:

    manager_t* parent;

    protected:

    base_t () :
        parent (NULL)
        {}

    base_t (manager_t* Aparent) :
        parent (Aparent)
        {}

    public:

    virtual ~base_t ()
        {
        //printf ("base_t::I disappeared\n");
        }

    virtual bool OnMouse (POINT mouse) { return true; }
    virtual void Draw () {}
    };

class rect_t : public base_t
    {
    public:
    POINT pos;
    POINT sz;

    protected:
    COLORREF color;

    public:

    rect_t () :
        base_t (NULL),
        pos (0, 0),
        sz  (0, 0)
        {}

    rect_t (POINT Apos, POINT Asz) :
        base_t (NULL),
        pos    (Apos),
        sz     (Asz)
        {}

    virtual ~rect_t ()
        {
        //printf ("rect_t::I disappeared\n");
        }

    virtual bool OnMouse (POINT mouse) override
        {
        printf ("Mouse: %d, %d; Pos: %d, %d, %d, %d\n", mouse.x, mouse.y, x1, y1, x2, y2);

        //if (Inside (mouse, x1, y1, x2, y2))
        if ((mouse.x >= x1) && (mouse.x < x2) && (mouse.y >= y1) && (mouse.y < y2))
            {
            printf ("Ok");

            color = TX_RED;

            if (txMouseButtons () % 2 == 1)
                return false;
            }
        else
            color = TX_GREEN;

        return true;
        }

    virtual void Draw () override
        {
        txSetFillColor (color);
        txRectangle (x1, y1, x2, y2);
        }
    };

//class button_t : public rect_t

class manager_t
    {
    public:

    std::vector <base_t*> elements;

    manager_t () :
        elements ()
        {}

    ~manager_t ()
        {
        for (base_t* element : elements)
            delete element;
        }

    void Add (base_t* new_element)
        {
        new_element->parent = *this;

        elements.push_back (new_element);
        }

    void Run ()
        {
        while (true)
            {
            for (base_t* element : elements)
                {
                if (element->OnMouse (txMousePos ()) == false)
                    break;

                element->Draw ();
                }

            txSleep (1);

            if (GetAsyncKeyState (VK_ESCAPE))
                break;
            }
        }
    };

//}

//===============================================================================================================

int main ()
    {
    txCreateWindow (1600, 850);

    manager_t manager;

    manager.Add (new rect_t (0, 0, 100, 100));

    manager.Run ();

    _txExit = true;

    return 0;
    }

bool Inside (POINT p, double x1, double y1, double x2, double y2)
    {
    if ((p.x >= x1) && (p.x < x2) && (p.y >= y1) && (p.y < y2))
        return true;
    else
        return false;
    }






















