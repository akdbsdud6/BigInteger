
//-----------------------------------------------------------------------------
// List.c
//
#include<iostream>
#include<string>
#include<stdexcept>
#include "List.h"

//Private Node constructor
List::Node::Node(ListElement x){
    data = x;
    next = nullptr;
    prev = nullptr;
}

//List constructor
List::List(){
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy;
    afterCursor = backDummy;

    pos_cursor = 0;
    num_elements = 0;
}

//copy List
List::List(const List& L){
    frontDummy = new Node(0);
    backDummy = new Node(0);
    this->frontDummy->next = backDummy;
    this->backDummy->prev = frontDummy;

    this->beforeCursor = frontDummy;
    this->afterCursor = backDummy;

    this->pos_cursor = 0;
    this->num_elements = 0;


    Node* Ltemp = L.frontDummy->next;
    while (Ltemp != L.backDummy){
        this->insertAfter(Ltemp->data);
        this->moveNext();
        Ltemp = Ltemp->next;
    }

}

//List destructor
List::~List(){
    Node* temp = frontDummy;
    while (temp != nullptr){
        Node* deletethis = temp;
        temp = temp->next;
        delete deletethis;
    }
}


//access functions

int List::length() const{
    return num_elements;
}

ListElement List::front() const{
    if (num_elements <= 0){
        throw std::length_error("List::front() : Invalid length\n");
    }
    return frontDummy->next->data;
}

ListElement List::back() const{
    if (num_elements <= 0){
        throw std::length_error("List::back() : Invalid length\n");
    }
    return backDummy->prev->data;
}

int List::position() const{
    int a = pos_cursor;
    if (a<0 || a >num_elements){
        throw std::range_error("List::Position() : Invalid cursor position\n");
    }else{
        return a;
    }
}

ListElement List::peekNext() const{
    if (pos_cursor >= num_elements){
        throw std::range_error("List::peekNext() : Invalid cursor position\n");
    }
    return afterCursor->data;
}

ListElement List::peekPrev() const{
    if (pos_cursor <= 0){
        throw std::range_error("List::peekPrev() : Invalid cursor position\n");
    }
    return beforeCursor->data;
}


//manipulation functions

void List::clear(){
    Node* temp = frontDummy->next;
    while (temp != backDummy){
        Node* deletethis = temp;
        temp = temp->next;
        delete deletethis;
    }
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    num_elements = 0;
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
}

void List::moveFront(){
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

void List::moveBack(){
    pos_cursor = num_elements;
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
}

ListElement List::moveNext(){
    if (num_elements <= pos_cursor){
        throw std::range_error("List::moveNext() : Invalid cursor position\n");
    }
    ListElement LE = afterCursor->data;
    pos_cursor += 1;
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;

    return LE;
}

ListElement List::movePrev(){
    if (pos_cursor <= 0){
        throw std::range_error("List::movePrev() : Invalid cursor position\n");
    }
    ListElement LE = beforeCursor->data;
    pos_cursor -= 1;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;

    return LE;
}


void List::insertAfter(ListElement x){
    Node* N = new Node(x);
    beforeCursor->next = N;
    afterCursor->prev = N;

    N->prev = beforeCursor;
    N->next = afterCursor;

    afterCursor = N;
    num_elements += 1;
}


// insertBefore()
// Inserts x before cursor.
void List::insertBefore(ListElement x){
    Node* N = new Node(x);
    beforeCursor->next = N;
    afterCursor->prev = N;

    N->prev = beforeCursor;
    N->next = afterCursor;

    beforeCursor = N;
    pos_cursor += 1;
    num_elements += 1;
}

// setAfter()
// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x){
    if (pos_cursor >= num_elements){
        throw std::range_error("List::setAfter() : Invalid cursor position\n");
    }
    afterCursor->data = x;

};

// setBefore()
// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x){
    if (pos_cursor <= 0){
        throw std::range_error("List::setBefore() : Invalid cursor position\n");
    }
    beforeCursor->data = x;
};

// eraseAfter()
// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter(){
    if (pos_cursor >= num_elements){
        throw std::range_error("List::eraseAfter() : Invalid cursor position\n");
    }

    Node* freethis = afterCursor;
    afterCursor = afterCursor->next;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    delete freethis;
    num_elements -= 1;
};

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore(){
    if (pos_cursor <= 0){
        throw std::range_error("List::eraseBefore() : Invalid cursor position\n");
    }

    Node* freethis = beforeCursor;
    beforeCursor = beforeCursor->prev;
    afterCursor->prev = beforeCursor;
    beforeCursor->next = afterCursor;
    delete freethis;
    pos_cursor -= 1;
    num_elements -= 1;
};


// Other Functions ---------------------------------------------------------

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then 
// returns the final cursor position. If x is not found, places the cursor 
// at position length(), and returns -1. 
int List::findNext(ListElement x){
    while (pos_cursor < num_elements){
        if (moveNext() == x){
            return pos_cursor;
        }
    }
    return -1;
};

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor 
// at position 0, and returns -1. 
int List::findPrev(ListElement x){
    while (pos_cursor > 0){
        if (movePrev() == x){
            return pos_cursor;
        }
    }
    return -1;
};

// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost 
// occurrance of each element, and removing all other occurances. The cursor 
// is not moved with respect to the retained elements, i.e. it lies between 
// the same two retained elements that it did before cleanup() was called.
void List::cleanup(){
    List* uniq = new List();
    //int original_cursor = pos_cursor;
    int cursor_copy = pos_cursor;
    moveFront();
    
    while (pos_cursor < num_elements){
        ListElement x = moveNext();
        if (uniq->findNext(x) != -1){ // x was already found
            if (pos_cursor <= cursor_copy){
                eraseBefore();
                cursor_copy -= 1;
            }else{                    
                eraseBefore();
            }
        }else{                       // x wasn't already found
            uniq->insertAfter(x);
        }
        uniq->moveFront();
    }

    moveFront();
    for (int i=0; i<cursor_copy; i++){
        moveNext();
    }
    delete uniq;
};

// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List& L) const{
    /* List* cat = new List();
    Node* temp = L.backDummy->prev;
    while (temp != L.frontDummy){
        cat->insertAfter(temp->data);
        //cat->moveNext();
        temp = temp->prev;
    }

    temp = this->backDummy->prev;
    while (temp != frontDummy){
        cat->insertAfter(temp->data);
        //cat->moveNext();
        temp = temp->prev;
    } */

    List cat = *this;
    Node* temp = L.backDummy->prev;
    cat.moveBack();
    while (temp != L.frontDummy){
        cat.insertAfter(temp->data);
        temp = temp->prev;
    }
    cat.moveFront();

    return cat;


    
    //cat->moveFront();

    //return *cat;
}

// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const{
    Node* temp = frontDummy->next;
    std::string s = "(";
    while (temp != backDummy){
        s += std::to_string(temp->data);
        temp = temp->next;
        if (temp != backDummy){
            s += ", ";
        }
    }
    s += ")";
    return s;
    
};

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const{
    if (this->length() != R.length()){
        return false;
    }
    Node* Ttemp = frontDummy->next;
    Node* Rtemp = R.frontDummy->next;
    while (Ttemp != backDummy){
        if (Ttemp->data != Rtemp->data){
            return false;
        }
        Ttemp = Ttemp->next;
        Rtemp = Rtemp->next;
    }

    return true;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ){
    return stream<<L.List::to_string();
};

// operator==()
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==( const List& A, const List& B ){
    return A.List::equals(B);
};

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ){
    if (this != &L){
        List temp = L;
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }

    return *this;
}
