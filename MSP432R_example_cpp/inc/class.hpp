/*
 * class.hpp
 *
 *  Created on: Feb 7, 2018
 *      Author: Andrew
 */

#ifndef INC_CLASS_HPP_
#define INC_CLASS_HPP_


class myClass
{
public:
    myClass();
    ~myClass();

    void setFoo(int newFoo) { foo = newFoo; }
    int getFoo(void) { return foo; }

private:
    int foo;
    char *bar;
};


#endif /* INC_CLASS_HPP_ */
