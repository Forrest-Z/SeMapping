#ifndef _HECTOR_MAP_PROC_CONTAINER_H_
#define _HECTOR_MAP_PROC_CONTAINER_H_

#include "../Map/GridMap.h"
#include "../Map/OccGridMapUtilConfig.h"
#include "../Matcher/ScanMatcher.h"
#include "../Utils/MapLockerInterface.h"

class GridMap;
class ConcreteOccGridMapUtil;
class DataContainer;

namespace NS_HectorMapping {

class MapProcContainer {
public:
	MapProcContainer(GridMap* gridMapIn,
			OccGridMapUtilConfig<GridMap>* gridMapUtilIn,
			ScanMatcher<OccGridMapUtilConfig<GridMap> >* scanMatcherIn) :
			gridMap(gridMapIn), gridMapUtil(gridMapUtilIn), scanMatcher(
					scanMatcherIn), mapMutex(0) {
	}

	virtual ~MapProcContainer() {
	}

	void cleanup() {
		delete gridMap;
		delete gridMapUtil;
		delete scanMatcher;

		if (mapMutex) {
			delete mapMutex;
		}
	}

	void reset() {
		gridMap->reset();
		gridMapUtil->resetCachedData();
	}

	void resetCachedData() {
		gridMapUtil->resetCachedData();
	}

	float getScaleToMap() const {
		return gridMap->getScaleToMap();
	}
	;

	const GridMap&
	getGridMap() const {
		return *gridMap;
	}
	;
	GridMap&
	getGridMap() {
		return *gridMap;
	}
	;

	void addMapMutex(MapLockerInterface* mapMutexIn) {
		if (mapMutex) {
			delete mapMutex;
		}

		mapMutex = mapMutexIn;
	}

	MapLockerInterface*
	getMapMutex() {
		return mapMutex;
	}

	Eigen::Vector3f matchData(const Eigen::Vector3f& beginEstimateWorld,
			const DataContainer& dataContainer, Eigen::Matrix3f& covMatrix,
			int maxIterations) {
		return scanMatcher->matchData(beginEstimateWorld, *gridMapUtil,
				dataContainer, covMatrix, maxIterations);
	}

	void updateByScan(const DataContainer& dataContainer,
			const Eigen::Vector3f& robotPoseWorld) {
		if (mapMutex) {
			mapMutex->lockMap();
		}

		gridMap->updateByScan(dataContainer, robotPoseWorld);

		if (mapMutex) {
			mapMutex->unlockMap();
		}
	}

	GridMap* gridMap;
	OccGridMapUtilConfig<GridMap>* gridMapUtil;
	ScanMatcher<OccGridMapUtilConfig<GridMap> >* scanMatcher;
	MapLockerInterface* mapMutex;
};

}

#endif
