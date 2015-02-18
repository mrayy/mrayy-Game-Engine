#include "stdafx.h"

#include "BenchmarkItem.h"
#include "ITimer.h"
#include "Engine.h"

namespace mray
{

BenchmarkItem::BenchmarkItem(const core::string&name):m_name(name),
m_samples(0),m_totalTime(0),m_enterTime(0),m_entered(false),m_lastTime(0)
{
}
BenchmarkItem::~BenchmarkItem()
{
	BenchmarkItemList::iterator it=m_subItems.begin();
	for(;it!=m_subItems.end();++it)
	{
		delete *it;
	}
	m_subItems.clear();
}

void BenchmarkItem::reset()
{
	m_samples=0;
	m_totalTime=0;
	m_entered=0;
	m_entered=false;
	m_lastTime=0;
	BenchmarkItemList::iterator it=m_subItems.begin();
	for(;it!=m_subItems.end();++it)
	{
		(*it)->reset();
	}
}

const core::string& BenchmarkItem::getName()const
{
	return m_name;
}

int BenchmarkItem::getSamplesCount()const
{
	return m_samples;
}
float BenchmarkItem::getTotalTime()const
{
	return m_totalTime;
}
float BenchmarkItem::getAverageTime()const
{
	if(m_samples>0)
		return m_totalTime/(float)m_samples;
	return m_totalTime;
}
float BenchmarkItem::getLastTime()const
{
	return m_lastTime;
}
const BenchmarkItem::BenchmarkItemList& BenchmarkItem::getSubItems()const
{
	return m_subItems;
}

void BenchmarkItem::OnEnter()
{
	if(!m_entered)
	{
		m_enterTime=gEngine.getTimer()->getSeconds();
		m_entered=true;
	}
}
float BenchmarkItem::OnExit()
{
	if(m_entered)
	{
		++m_samples;
		m_lastTime = gEngine.getTimer()->getSeconds() - m_enterTime;
		m_totalTime+=m_lastTime;

		m_entered=false;
	}
	return m_lastTime;
}

BenchmarkItem* BenchmarkItem::getSubItem(const core::string&name)
{
	BenchmarkItemList::iterator it=m_subItems.begin();
	for(;it!=m_subItems.end();++it)
	{
		if((*it)->getName().equals_ignore_case(name))
			return *it;
	}
	return 0;
}

BenchmarkItem* BenchmarkItem::addSubItem(const core::string&name)
{
	BenchmarkItem *res=new BenchmarkItem(name);
	m_subItems.push_back(res);
	return res;
}

void BenchmarkItem::addSubItem(BenchmarkItem* item)
{
	m_subItems.push_back(item);
}
void BenchmarkItem::removeSubItem(BenchmarkItem* item)
{
	BenchmarkItemList::iterator it=m_subItems.begin();
	for(;it!=m_subItems.end();++it)
	{
		if((*it)==item)
		{
			m_subItems.erase(it);
			return;
		}
	}

}

}
