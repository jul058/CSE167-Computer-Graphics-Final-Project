
#include "Group.hpp"
Group::Group()
{
    
}

void Group::addChild(Node* node)
{
    children.push_back(node);
}


void Group::removeChild(Node* node)
{
    for(vector<Node*>::iterator it = children.begin(); it!=children.end(); ++it)
    {
        if(*it == node)
        {
            children.erase(it);
            break;
        }
        
    }
}

void Group::draw(GLuint skyboxShader)
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->draw(skyboxShader);
    }
}

void Group::update(glm::mat4 C)
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->update(C);
    }
}