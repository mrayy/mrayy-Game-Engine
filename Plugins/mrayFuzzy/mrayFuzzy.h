
/********************************************************************
	created:	2010/03/25
	created:	25:3:2010   19:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\mrayFuzzy.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	mrayFuzzy
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___mrayFuzzy___
#define ___mrayFuzzy___


#include "FuzzyModule.h"

//Sets
#include "FuzzyLeftShoulderSet.h"
#include "FuzzyRightShoulderSet.h"
#include "FuzzySingletonSet.h"
#include "FuzzyTrapezoidalSet.h"
#include "FuzzyTriangleSet.h"
#include "FuzzySetProxy.h"

//Defuzzy
#include "DefuzzyMaxAv.h"
#include "DefuzzyCentroid.h"

//Operators
#include "FuzzyAndOpt.h"
#include "FuzzyNotOpt.h"
#include "FuzzyOrOpt.h"

//Hedges
#include "FuzzyVeryHedge.h"
#include "FuzzyFairlyHedge.h"

//Clustering
#include "FCMClustering.h"

//parsers
#include "FuzzyXMLParser.h"

#endif //___mrayFuzzy___
