#include <wtypes.h>
#include "TaskCollection.h"
#include "BaseTask.h"

using namespace std;

TaskCollection::TaskCollection(bool topOnly /*= true */)
	:mProcessTopOnly( topOnly )
{
	mTasks = new std::vector<BaseTask*>;
}

TaskCollection::~TaskCollection()
{
	vector<BaseTask*>::iterator iter;
	for( iter = mTasks->begin(); iter!= mTasks->end(); ++iter )
	{
		delete (*iter);
	}

	delete mTasks;
}

bool TaskCollection::update()
{
	if (anyTasksDone())//cleanup any completed tasks first
	{
		removeCompletedTasks();
	}

	bool anyDone = false;
	vector<BaseTask*>::iterator iter;
	for( iter = mTasks->begin(); iter!= mTasks->end(); ++iter )
	{
		anyDone |= (*iter)->update();
		if( mProcessTopOnly )
			break;
	}

	if( anyDone )
	{
		removeCompletedTasks();
	}
	return mTasks->empty();
}

void TaskCollection::addTask(BaseTask* pTask)
{
	mTasks->push_back( pTask );
}

void TaskCollection::removeCompletedTasks()
{
	//don't bother if there's nothing in the vector
	if( !mTasks->empty() )
	{
		//create temp vector
		vector<BaseTask*>* pTemp = new vector<BaseTask*>;

		//go through old tasks copying ones that shouldn't be removed
		vector<BaseTask*>::iterator iter;
		for( iter = mTasks->begin(); iter!= mTasks->end(); ++iter )
		{
			BaseTask* pTask = *iter;
			if( !pTask->isDone() )
			{
				pTemp->push_back( pTask );
			}
			else
			{
				delete pTask;
			}
		}

		delete mTasks;
		mTasks = pTemp;
	}
}

void TaskCollection::removeAllTasks()
{
	vector<BaseTask*>::iterator iter;
	for (iter = mTasks->begin(); iter != mTasks->end(); ++iter)
	{
		delete (*iter);
	}

	mTasks->clear();
}

void TaskCollection::markAllTasksDone()
{
	vector<BaseTask*>::iterator iter;
	for (iter = mTasks->begin(); iter != mTasks->end(); ++iter)
	{
		(*iter)->flagAsDone();
	}
}

bool TaskCollection::anyTasksDone() const
{
	vector<BaseTask*>::iterator iter;
	for (iter = mTasks->begin(); iter != mTasks->end(); ++iter)
	{
		if ((*iter)->isDone())
		{
			return true;
		}
	}
	return false;
}
