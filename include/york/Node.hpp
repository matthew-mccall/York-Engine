#if !defined(YORK_NODE_HPP)
#define YORK_NODE_HPP

#include <concepts>
#include <vector>

namespace york {

class NodeBase;

template <typename T>
concept ChildNode = std::is_base_of_v<NodeBase, T>;

class NodeBase {
private:
    std::vector<NodeBase*> m_children;

public:
    NodeBase();
    void addChild(NodeBase* node);

    template <ChildNode NodeT, typename... ArgsT>
    NodeT* createChild(ArgsT&&... args)
    {
        NodeT* node = new NodeT(std::forward<ArgsT>(args)...);
        m_children.push_back(node);
        return node;
    }

    virtual ~NodeBase();
};

template <typename T>
class Node : public NodeBase {
private:
    T x;

public:
    Node(T x, NodeBase& parent);

    T* operator->()
    {
        return &x;
    }
};

} // namespace york

#endif // YORK_NODE_HPP
