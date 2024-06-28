#pragma once
#include <iostream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <stdexcept>
#include <memory>
#include <vector>
#include <cfloat>
#include <mutex>
#include <shared_mutex>

class TreeNode : public std::enable_shared_from_this<TreeNode>{
private:
	std::shared_ptr<TreeNode> leftNode;
	std::shared_ptr<TreeNode> rightNode;
	
	double leftB;
	double rightB;
	double midB;
	double value;

protected:
	std::shared_ptr<TreeNode> NextNode;
	std::shared_ptr<TreeNode> LastNode;

public:

	TreeNode(double leftB, double rightB, double value, std::shared_ptr<TreeNode> NextNode = nullptr, std::shared_ptr<TreeNode> LastNode = nullptr) {
		this->leftB = leftB;
		this->rightB = rightB;
		this->value = value;
		this->midB = -1;
		this->leftNode = this->rightNode = nullptr;

		this->NextNode = NextNode;
		this->LastNode = LastNode;
	}

	static std::shared_ptr<TreeNode> Create(double leftB, double rightB, double value, std::shared_ptr<TreeNode> NextNode = nullptr, std::shared_ptr<TreeNode> LastNode = nullptr) {
		auto Prt = std::make_shared< TreeNode>(leftB, rightB, value, NextNode, LastNode);
		if (LastNode != nullptr)
			Prt->LastNode->setNextNode(Prt);
		if (NextNode != nullptr)
			Prt->NextNode->setLastNode(Prt);
		return Prt;
	}

	TreeNode(std::shared_ptr<TreeNode> leftNode, double rightB, double value, std::shared_ptr<TreeNode> NextNode = nullptr, std::shared_ptr<TreeNode> LastNode = nullptr) {
		this->leftB = leftNode->getLeftB();
		this->rightB = rightB;
		this->value = value < leftNode->getValue()?value:leftNode->getValue();
		this->midB = leftNode->getRightB();
		this->leftNode = leftNode;
		this->rightNode = TreeNode::Create(midB, rightB, value, NextNode, LastNode);
	}

	void Delete();

	static void Delete(std::shared_ptr<TreeNode> Node) {
		Node->Delete();
	}

	static std::shared_ptr<TreeNode> Create(std::shared_ptr<TreeNode> leftNode, double rightB, double value, std::shared_ptr<TreeNode> NextNode = nullptr, std::shared_ptr<TreeNode> LastNode = nullptr) {
		auto Prt = std::make_shared< TreeNode>(leftNode, rightB, value, NextNode, LastNode);
		return Prt;
	}

	void Slice(double B);
	void addValue(double LB, double RB, double value);
	void setNextNode(std::shared_ptr<TreeNode> NextList) { 
		this->NextNode = NextList;
		//NextList->setLastNode(this);
	}
	void setLastNode(std::shared_ptr<TreeNode> LastNode) { 
		this->LastNode = LastNode;
		//LastNode->setNextNode(this);
	}
	double getLeftB();
	double getRightB();
	double getValue() ;
	double getValue(double point) ;
	double getRightValue(double point) ;
	std::shared_ptr<TreeNode> getInterval(double point) ;
	double getRightArea() ;
	double getLeftArea() ;
	std::shared_ptr<TreeNode> getNextNode() ;
	std::shared_ptr<TreeNode> getLastNode() ;
	std::shared_ptr<TreeNode> releaseLeft(double B);
};

class IntervalTree {
private:
	std::shared_ptr<TreeNode> root;
	std::shared_ptr<TreeNode> List;
	double lastLeftB;
	double windows;
	double defualtValue;
	std::shared_mutex UpdateLock;
	thread_local int _hasShare = 0;
	thread_local int _hasUnique = 0;
	void shareLock();
	void releaseShareLock();
	void uniqueLock();
	void releaseUniqueLock();
	// 
protected:
	std::shared_ptr<TreeNode> getInterval(double point);
	void Slice(double B);
	void extend(double rightB);
	void extend(double rightB, double value);
public:
	IntervalTree(double defualtValue, double lastLeftB = 0, double windows = 30) {
		this->root = std::make_shared<TreeNode>(lastLeftB, windows, defualtValue);
		this->List = std::make_shared<TreeNode>(-1, -1, -1);
		this->root->setNextNode(this->List);
		this->root->setLastNode(this->List);
		this->List->setNextNode(this->root);
		this->List->setLastNode(this->root);
		this->windows = windows;
		this->lastLeftB = lastLeftB;
		this->defualtValue = defualtValue;
	}
	void addValue(double LB, double RB, double value);
	std::pair<double,double> addContinueValue(double LB,double Size);
	std::pair<double,double> addContinueValue(std::vector<double>& interval,double Size);
	double getValue(double point) ;
	double getRangeArea(double start,double end) ;
	void releaseLeft(double B);
	void intoNextWindows();
	void intoNextWindows(double x);
	void changeDefualtValue(double now,double Value);
	double ContinuousAllocated(double startPoint, double targetArea);
	double ContinuousAllocated(double startPoint, double targetArea,std::vector<double>& ans);
	double AllocatedArea_DD(double startPoint, double targetArea);
	double AllocatedArea_DDD(double startPoint, double targetArea, double hight);
	std::string viewList();
};