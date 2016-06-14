#include "MatrixTransform.hpp"

MatrixTransform::MatrixTransform()
{
   M = glm::mat4(1.0f);
}


void MatrixTransform::update(glm::mat4 C)
{
    Group::update(C*M);
}

