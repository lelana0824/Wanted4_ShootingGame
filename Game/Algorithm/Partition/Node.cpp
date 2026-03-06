#include "Node.h"
#include "QuadTree.h"

Node::Node(const Bounds& bounds, int depth)
    : bounds(bounds), depth(depth)
{

}
Node::~Node()
{
    Clear();
}

void Node::Insert(Node* node)
{
    if (!node) return;

    // 겹치는 영역 확인
    NodeIndex result = TestRegion(node->GetBounds());

    // 두 개 이상 영역에 겹치는 경우에는 현재 노드에 추가.
    if (result == NodeIndex::Straddling)
    {
        points.emplace_back(node);
    }

    // 겹치지 않은 경우
    // 경우의 수 1: 1개의 영역에만 포함되는 경우.
    else if (result != NodeIndex::OutOfArea)
    {
        // Subdivide 호출 시 MaxDepth에 도달하지 않으면 분할 진행.
        if (Subdivide())
        {
            if (result == NodeIndex::TopLeft)
            {
                topLeft->Insert(node);
            } 
            else if (result == NodeIndex::TopRight)
            {
                topRight->Insert(node);
            }
            else if (result == NodeIndex::BottomLeft)
            {
                bottomLeft->Insert(node);
            }
            else if (result == NodeIndex::BottomRight)
            {
                bottomRight->Insert(node);
            }
        }

        // 분할 할 수 없는 경우 (이미 최대 깊이)
        else
        {
            points.emplace_back(node);
        }
    }
    
    // 경우의 수2: 영역 밖에 있는 경우(OutOfArea). 아무 처리 안함.
}

void Node::Query(
    const Bounds& bounds, 
    std::vector<Node*>& possibleNodes)
{
    // 현재 노드를 추가하고 이후 과정 진행.
    possibleNodes.emplace_back(this);

    // 분할 여부 확인 후 자손 검사
    if (!IsDivided())
    {
        return;
    }

    // 전달 받은 영역과 겹치는 4분면 목록 확인.
    std::vector<NodeIndex> quads = GetQuads(bounds);

    // 검사 진행
    for (const NodeIndex& index : quads)
    {
        if (index == NodeIndex::TopLeft)
        {
            topLeft->Query(bounds, possibleNodes);
        }
        else if (index == NodeIndex::TopRight)
        {
            topRight->Query(bounds, possibleNodes);
        }
        else if (index == NodeIndex::BottomLeft)
        {
            bottomLeft->Query(bounds, possibleNodes);
        }
        else if (index == NodeIndex::BottomRight)
        {
            bottomRight->Query(bounds, possibleNodes);
        }
    }
}

void Node::Clear()
{
    // points에 추가되는 노드는 외부에서 관리하는 노드이기 때문에
    // 여기에서는 메모리 관리하지않음.

    // 목록정리
    points.clear();

    // 분할된 경우 자손 정리
    if (IsDivided())
    {
        topLeft->Clear();
        topRight->Clear();
        bottomLeft->Clear();
        bottomRight->Clear();

        ClearChildren();
    }
}

bool Node::Subdivide()
{
    // 최대 깊이 (임시값 사용) 도달했는지 확인
    if (depth == 5)
    {
        return false;
    }

    // 분할 여부 파악
    // 분할 전이면 영역 나누기.
    if (!IsDivided())
    {
        int x = bounds.X();
        int y = bounds.Y();
        int halfWidth = bounds.Width() / 2;
        int halfHeight = bounds.Height() / 2;

        // 각 4분면의 자식 객체 생성
        topLeft = new Node(
            Bounds(x, y, halfWidth, halfHeight),
            depth + 1
        );

        topRight = new Node(
            Bounds(x + halfWidth, y, halfWidth, halfHeight),
            depth + 1
        );

        bottomLeft = new Node(
            Bounds(x, y + halfHeight, halfWidth, halfHeight),
            depth + 1
        );

        bottomRight = new Node(
            Bounds(x + halfWidth, y + halfHeight, halfWidth, halfHeight),
            depth + 1
        );
    }
    return true;
}

bool Node::IsDivided()
{
    // 분할할때는 모든 자식노드가 null이 아니기 때문에 
    // 하나의 자식만 파악해도 됨.
    return topLeft != nullptr;
}

NodeIndex Node::TestRegion(const Bounds& bounds)
{
    std::vector<NodeIndex> quads = GetQuads(bounds);

    // 겹치는 곳이 없으면 OOA 반환.
    if (quads.empty()) return NodeIndex::OutOfArea;

    // 4개중 하나의 영역에만 겹치는 경우 해당 4분면 반환.
    if (quads.size() == 1) return quads[0];

    // 여러 영역과 겹친 경우
    return NodeIndex::Straddling;
}

std::vector<NodeIndex> Node::GetQuads(const Bounds& bounds)
{
    // 겹치는 영역 반환을 위한 변수
    std::vector<NodeIndex> quads;

    // 영역 계산에 필요한 변수.
    int x = this->bounds.X();
    int y = this->bounds.Y();
    int halfWidth = this->bounds.Width() / 2;
    int halfHeight = this->bounds.Height() / 2;
    int centerX = x + halfWidth;
    int centerY = y + halfHeight;

    // 왼쪽 영역이랑 겹치는지 확인.
    bool left
        = bounds.X() < centerX && bounds.MaxX() >= x;

    // 오른쪽 영역이랑 겹치는지 확인.
    bool right
        = bounds.MaxX() >= centerX && bounds.X() < this->bounds.MaxX();

    // 위쪽 영역이랑 겹치는지 확인.
    bool top = bounds.Y() < centerY && bounds.MaxY() >= y;

    // 아래쪽 영역이랑 겹치는지 확인.
    bool bottom
        = bounds.MaxY() >= centerY && bounds.Y() < this->bounds.MaxY();

    // 영역 판정.
    if (top && left)
    {
        quads.emplace_back(NodeIndex::TopLeft);
    }
    if (top && right)
    {
        quads.emplace_back(NodeIndex::TopRight);
    }
    if (bottom && left)
    {
        quads.emplace_back(NodeIndex::BottomLeft);
    }
    if (bottom && right)
    {
        quads.emplace_back(NodeIndex::BottomRight);
    }

    return quads;
}

void Node::ClearChildren()
{
    if (IsDivided())
    {
        SafeDelete(topLeft);
        SafeDelete(topRight);
        SafeDelete(bottomLeft);
        SafeDelete(bottomRight);
    }
}
