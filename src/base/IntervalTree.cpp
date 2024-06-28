
#include "intervalTree.h"

void TreeNode::Delete() {
	if (NextNode != nullptr)
		NextNode->setLastNode(LastNode);
	if (LastNode != nullptr)
		LastNode->setNextNode(NextNode);
}

void TreeNode::Slice(double B)
{
	if (B <= leftB || B >= rightB)
	{
		return;
	}
	if (leftNode == nullptr && rightNode == nullptr)
	{
		std::shared_ptr<TreeNode> NNode = NextNode; // 叶子节点组成链表
		std::shared_ptr<TreeNode> LNode = LastNode; // 叶子节点组成链表
		LastNode = NextNode = nullptr;
		this->midB = B;

		this->leftNode = TreeNode::Create(leftB, midB, value);
		this->rightNode = TreeNode::Create(midB, rightB, value);
		this->leftNode->setLastNode(LNode);
		this->leftNode->setNextNode(this->rightNode);
		this->rightNode->setLastNode(this->leftNode);
		this->rightNode->setNextNode(NNode);
		LNode->setNextNode(leftNode);
		NNode->setLastNode(rightNode);
	}
	else
	{
		if (B <= midB)
		{
			this->leftNode->Slice(B);
		}
		else
		{
			this->rightNode->Slice(B);
		}
	}

}

void TreeNode::addValue(double LB, double RB, double AddValue)
{
	if (LB >= rightB || RB < leftB)
	{
		throw std::exception();
	}
	if (LB == leftB && RB == rightB)
	{
		this->value += AddValue;
		if (this->value < -0.001)
		{
			std::cout << "value < 0" << std::endl;
			throw std::exception();
		}
		if (this->rightNode != nullptr)
			this->rightNode->addValue(midB, RB, AddValue);
		if (this->leftNode != nullptr)
			this->leftNode->addValue(LB, midB, AddValue);
		return;
	}
	else
	{
		if (leftNode == nullptr || rightNode == nullptr)
		{
			double MB = abs(2 * LB - leftB - rightB) > abs(2 * RB - leftB - rightB) ? RB : LB;
			this->Slice(MB);
		}
		if (RB <= midB)
		{
			this->leftNode->addValue(LB, RB, AddValue);
		}
		else if (LB >= midB)
		{
			this->rightNode->addValue(LB, RB, AddValue);
		}
		else
		{
			this->leftNode->addValue(LB, midB, AddValue);
			this->rightNode->addValue(midB, RB, AddValue);
		}
		double Lvalue = leftNode->getValue();
		double Rvalue = rightNode->getValue();
		value = Lvalue > Rvalue ? Rvalue : Lvalue;
	}
}

double TreeNode::getLeftB() 
{
	return leftB;
}

double TreeNode::getRightB() 
{
	return rightB;
}

double TreeNode::getValue() 
{
	return value;
}

double TreeNode::getValue(double point) 
{
	if (point < leftB || point > rightB)
	{
		return -1;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return getValue();
	}
	else
	{
		if (point < midB)
			return leftNode->getValue(point);
		else
			return rightNode->getValue(point);
	}
}


double TreeNode::getRightValue(double point)  {
	// 获取该点往右的最小值
	if (point < leftB || point > rightB)
	{
		return -1;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return getValue();
	}
	else
	{
		if (point < midB)
		{
			double Lvalue = leftNode->getValue(point);
			double Rvalue = rightNode->getValue();
			return Lvalue > Rvalue ? Rvalue : Lvalue;
		}
		else
			return rightNode->getRightValue(point);
	}


}

std::shared_ptr<TreeNode> TreeNode::getInterval(double point) 
{
	if (point < leftB || point > rightB)
	{
		return nullptr;
	}
	if (leftNode == nullptr || rightNode == nullptr)
	{
		return shared_from_this();
	}
	else
	{
		if (point <= midB)
			return leftNode->getInterval(point);
		else
			return rightNode->getInterval(point);
	}
}

double TreeNode::getRightArea() 
{
	if (this->rightNode != nullptr)
		return this->rightNode->getRightArea();
	else
		return this->value * (this->rightB - this->leftB);
}

double TreeNode::getLeftArea() 
{
	if (this->leftNode != nullptr)
		return this->leftNode->getLeftArea();
	else
		return this->value * (this->rightB - this->leftB);
}

std::shared_ptr<TreeNode> TreeNode::getNextNode() 
{
	return NextNode;
}
std::shared_ptr<TreeNode> TreeNode::getLastNode() 
{
	return LastNode;
}

std::shared_ptr<TreeNode> TreeNode::releaseLeft(double B)
{
	if (leftNode == nullptr && rightNode == nullptr)
	{
		if (rightB <= B)
		{
			return nullptr;
		}
		else
		{
			leftB = B > leftB ? B : leftB;
			return shared_from_this();
		}
	}
	else
	{
		if (B <= midB)
		{
			std::shared_ptr<TreeNode> left = nullptr;
			left = this->leftNode->releaseLeft(B);
			if (left != this->leftNode)
			{
				leftNode->Delete();
				leftNode = left;
			}
		}
		else
		{
			std::shared_ptr<TreeNode> right = nullptr;
			right = rightNode->releaseLeft(B);
			if (right != this->rightNode)
			{
				leftNode->Delete();
				rightNode->Delete();
				rightNode = right;
				leftNode = nullptr;
			}
		}
		if (leftNode == nullptr) {
			std::shared_ptr<TreeNode> r = rightNode;
			rightNode = nullptr;
			return r;
		}
		else
		{
			double Lvalue = leftNode == nullptr ? LDBL_MAX : leftNode->getValue();
			double Rvalue = rightNode == nullptr ? -1 : rightNode->getValue();
			value = Lvalue > Rvalue ? Rvalue : Lvalue;
			leftB = leftNode->getLeftB();
			return shared_from_this();
		}
	}
}

void IntervalTree::shareLock(){
	if(_hasUnique == 0 && _hasShare == 0){
		UpdateLock.lock_shared();
	}
	_hasShare += 1;
} 

void IntervalTree::uniqueLock(){
	if(_hasShare != 0){
		UpdateLock.unlock_shared();
	}
	if(_hasUnique == 0){
		UpdateLock.lock();
	}
	_hasUnique += 1;
}

void IntervalTree::releaseShareLock(){
	_hasShare -= 1;
	if(_hasUnique == 0 && _hasShare == 0){
		UpdateLock.unlock_shared();
	}
}


void IntervalTree::releaseUniqueLock(){
	_hasUnique --;
	if(_hasUnique == 0){
		UpdateLock.unlock();
	}
	if(_hasShare != 0){
		UpdateLock.lock_shared();
	}
}



std::shared_ptr<TreeNode> IntervalTree::getInterval(double point) 
{
	return root->getInterval(point);
}

void IntervalTree::Slice(double B) {
	uniqueLock();
	extend(B);
	this->root->Slice(B);
}


void IntervalTree::extend(double rightB)
{
	uniqueLock();
	while (this->root->getRightB() < rightB) {
		auto Last = List->getLastNode();
		root = TreeNode::Create(root, rightB, defualtValue, List, Last);
	}
}

void IntervalTree::extend(double rightB, double value)
{
	uniqueLock();
	while (this->root->getRightB() < rightB) {
		auto Last = List->getLastNode();
		root = TreeNode::Create(root, rightB, value, List, Last);
	}
}

void IntervalTree::addValue(double LB, double RB, double value)
{
	uniqueLock();
	double rightB = root->getRightB();
	double LeftB = root->getLeftB();
	if (rightB < RB)
	{
		intoNextWindows(RB);
	}
	if (LeftB > LB)
	{
		LB = LeftB;
	}
	this->root->addValue(LB, RB, value);
}

std::pair<double,double> IntervalTree::addContinueValue(double LB,double Size){
	shareLock();
	intoNextWindows(LB);
	std::vector<double> interval;
	ContinuousAllocated(LB,Size,interval);
	double start,sec,value;
	int i = 0; 
	start = interval[i++];
	do{
		if(i<interval.size()){
			value = interval[i++];
		}else{
			value = List->getLastNode()->getValue();
		}
		if(i<interval.size()){
			sec = interval[i++];
		}else{
			sec = start + Size/value;
		}
		addValue(start,sec,-1*value);
		start = sec;
	}while(i<interval.size());
	return std::pair<double,double>(interval[0],sec);
}

std::pair<double,double> IntervalTree::addContinueValue(std::vector<double>& interval,double Size){
	shareLock();
	double start,sec,value;
	int i = 0; 
	start = interval[i++];
	while(Size > 0){
		if(i<interval.size()){
			value = interval[i++];
			sec = interval[i++];
		}else{
			value = List->getLastNode()->getValue();
			sec = start + Size/value;
		}
		addValue(start,sec,-1*value);
		start = sec;
		Size -= (sec - start) * value;
	}
	return std::pair<double,double>(interval[0],sec);
}

double IntervalTree::getValue(double point) 
{
	shareLock();
	return this->root->getValue(point);
}

double IntervalTree::getRangeArea(double start,double end) {
	shareLock();
	double RB = root->getRightB();
	double Area = 0;
	if(RB < end){
		double value = List->getLastNode()->getValue();
		Area += (end - RB)*value;
		end = RB;
	}
	std::shared_ptr<TreeNode> Node = getInterval(start);
	double lastPoint = start;
	while (Node != List && Node != nullptr && lastPoint < end)
	{
		double rightB = Node->getRightB();
		if(rightB > end){
			rightB = end;
		}
		Area += Node->getValue() * (rightB - lastPoint);
		lastPoint = rightB;
		Node = Node->getNextNode();
	}
	return Area ;
}

void IntervalTree::releaseLeft(double B) {
	uniqueLock();
	intoNextWindows(B);
	double RealseB = int((B - lastLeftB) / windows - 1) * windows + lastLeftB;
	double LB = root->getLeftB();
	if (RealseB > LB) {
		root->releaseLeft(RealseB);
	}
}

void IntervalTree::intoNextWindows()
{
	uniqueLock();
	double Right = (int((root->getRightB() - lastLeftB) / windows) + 1) * windows + lastLeftB;
	extend(Right);
}

void IntervalTree::intoNextWindows(double x)
{
	uniqueLock();
	if (x >= root->getRightB()) {
		double Right = (int((x - lastLeftB) / windows) + 1) * windows + lastLeftB;
		extend(Right);
	}
}

void IntervalTree::changeDefualtValue(double now, double Value) {
	intoNextWindows(now);
	
	shareLock();

	double RB = root->getRightB();
	double Pvalue = root->getRightValue(now);
	if (Pvalue < defualtValue - Value && Pvalue != defualtValue) {
		throw std::range_error("now has Task running");
	}

	releaseShareLock();

	addValue(now, RB, Value - defualtValue);

	uniqueLock();
	defualtValue = Value;
	releaseUniqueLock();
}


double IntervalTree::ContinuousAllocated(double startPoint, double targetArea){
	// 返回的值为最接近(往后)满足高为hight，面积为targetArea的区块的起点

	intoNextWindows(startPoint);

	shareLock();
	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double leftArea = 0;
	double ansPoint = startPoint;
	while (Node != List && Node != nullptr)
	{
		if (Node->getValue() <= 0)
		{
			leftArea = 0;
			ansPoint = Node->getRightB();
		}
		else
		{
			leftArea *= Node->getValue() * (Node->getRightB() - Node->getLeftB());
		}
		if (leftArea >= targetArea)
		{
			return ansPoint;
		}
		Node = Node->getNextNode();
	}
	releaseShareLock();
	return List->getRightB();
}

double IntervalTree::ContinuousAllocated(double startPoint, double targetArea, std::vector<double>& ans){
	// 返回的值为最接近(往后)满足高为hight，面积为targetArea的区块的起点

	intoNextWindows(startPoint);

	shareLock();
	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double leftArea = 0;
	double ansPoint = startPoint;
	ans.push_back(ansPoint);
	while (Node != List && Node != nullptr)
	{
		if (Node->getValue() <= 0)
		{
			leftArea = 0;
			ansPoint = Node->getRightB();
			ans.clear();
			ans.push_back(ansPoint);
		}
		else
		{
			leftArea += Node->getValue() * (Node->getRightB() - Node->getLeftB());
			if (leftArea >= targetArea)
			{
				ans.push_back(Node->getValue());
				ans.push_back(Node->getRightB() - (leftArea - targetArea)/Node->getValue());
				return ansPoint;
			}else{
				ans.push_back(Node->getValue());
				ans.push_back(Node->getRightB());
			}
		}
		Node = Node->getNextNode();
	}
	releaseShareLock();
	return ansPoint;
}

double IntervalTree::AllocatedArea_DD(double startPoint, double targetArea)
{
	// 返回的值为从startPoint开始分配的高
	shareLock();
	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double hight = Node == List ? -1 : DBL_MAX;
	// std::cout << viewList() << std::endl;
	while (Node != List && hight > 0.001 && Node != nullptr)
	{
		double RB = Node->getRightB();
		double value = Node->getValue();
		hight = hight > value ? value : hight;
		if ((RB - startPoint) * hight >= targetArea || hight < 0.001)
			break;
		Node = Node->getNextNode();
	}
	releaseShareLock();
	return Node == List || hight < 0.001 || Node == nullptr ? -1 : hight;
}

double IntervalTree::AllocatedArea_DDD(double startPoint, double targetArea, double hight)
{
	shareLock();
	// 返回的值为最接近(往后)满足高为hight，面积为targetArea的区块的起点
	if (hight <= 0){
		releaseShareLock();
		return root->getRightB();
	}

	intoNextWindows(startPoint);

	std::shared_ptr<TreeNode> Node = getInterval(startPoint);
	double leftArea = 0;
	double ansPoint = startPoint;
	while (Node != List && Node != nullptr)
	{
		if (Node->getValue() < hight)
		{
			leftArea = 0;
			ansPoint = Node->getRightB();
		}
		else
		{
			leftArea = hight * (Node->getRightB() - ansPoint);
		}
		if (leftArea >= targetArea)
		{
			return ansPoint;
		}
		Node = Node->getNextNode();
	}
	releaseShareLock();
	return hight > defualtValue ? -1 : ansPoint;
}

std::string IntervalTree::viewList() 
{
	std::ostringstream stream;
	auto NextNode = List->getNextNode();
	while (NextNode != List)
	{
		stream << "(" << NextNode->getLeftB() << "," << NextNode->getRightB() << "," << NextNode->getValue() << ")";
		NextNode = NextNode->getNextNode();
	}
	return stream.str();
}

