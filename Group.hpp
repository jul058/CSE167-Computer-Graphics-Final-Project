

#include "Node.hpp"

using namespace std;
//class Node;
class Group : public Node
{
public:
    Group();
    vector<Node*> children;
    void addChild(Node* node);
    void removeChild(Node* node);
    virtual void draw(GLuint skyboxShader);
    virtual void update(glm::mat4 C);
};