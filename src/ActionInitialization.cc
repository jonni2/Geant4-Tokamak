#include "ActionInitialization.hh"

MyActionInitialization::MyActionInitialization() {}

MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::Build() const {
    MyPrimaryGeneratorAction* generator = new MyPrimaryGeneratorAction();
    
    SetUserAction(generator);
}
