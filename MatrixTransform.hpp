#include "Group.hpp"
class MatrixTransform : public Group
{
public:
    glm::mat4 M;
    MatrixTransform();
    void update(glm::mat4 C);
};


