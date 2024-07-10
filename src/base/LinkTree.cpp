#include "LinkTree.h"

bool LinkTree::Update(double endTime)
{
    bool ans = true;
    while (endTime > LastUpdate)
    {
        double rate = this->link->getRate(LastUpdate);
        if (rate > 0)
            LinkB->changeDefualtValue(LastUpdate, rate);
        else
        {
            ans = false;
            break;
        }
        LastUpdate += stepTime;
    }
    return ans;
}

void LinkTree::releasLeft(double now)
{
    double last = int((now - stepTime) / stepTime);
    if (last >= LastLeft)
    {
        LinkB->releaseLeft(last);
        LastLeft = last;
    }
}

double LinkTree::getRangeArea(double start, double end)
{
    return LinkB->getRangeArea(start, end);
}

std::pair<double, double> LinkTree::trans(double start, double end, double value)
{
    // releasLeft(start);
    bool has = Update(end);
    if (has)
        LinkB->addValue(start, end, -1 * value);
    return std::pair<double, double>(start, has ? end + this->link->getDelay(start) : DBL_MAX);
}

std::pair<double, double> LinkTree::trans(double start, double size)
{
    // releasLeft(start);
    std::vector<double> interval;
    LinkB->ContinuousAllocated(start, size, interval);
    double endB = interval.at(interval.size()-1);
    bool has = true;
    // 更新区间，防止区间树未申请区域，导致后续速率为负数
    while (has && endB == LinkB->getRightB())
    {
        has = Update(endB + size / LinkB->getDefualtValue());
        LinkB->ContinuousAllocated(start, size, interval);
        endB = interval.at(interval.size()-1);
    }

    std::pair<double, double> ans;
    // 如果更新时发现数据错误，则消息损毁
    if (has)
    {
        ans = LinkB->addContinueValue(interval, size);
        ans.second += this->link->getDelay(start);
    }
    else
    {
        ans = std::make_pair(start, DBL_MAX);
    }
    return ans;
}