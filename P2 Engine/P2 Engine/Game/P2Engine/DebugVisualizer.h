#ifndef DEBUGVISUALIZER_H
#define DEBUGVISUALIZER_H

#include <list>
#include <stack>
#include "Azul.h"

class DebugVisualizer{

	friend class Game;
private:

	static DebugVisualizer* instance;

	struct visualData{

		GraphicsObjectWireFrame* gObj;
		Matrix worldMat;
		Vect color;

		visualData(): gObj(0), worldMat(Matrix(IDENTITY)), color(Vect(0,0,0)){}
	};

	typedef std::list<visualData*> visualDataCollection;
	visualDataCollection visualRequests;
	std::stack<visualData*> visDataPool;

	GraphicsObjectWireFrame* wfSphere;
	GraphicsObjectWireFrame* wfBox;

	DebugVisualizer(){}
	DebugVisualizer(const DebugVisualizer&){}
	~DebugVisualizer(){}

	static DebugVisualizer& GetInstance(){
		if(!instance){
			instance = new DebugVisualizer();
		}
		return *instance;
	}

	static Vect DEFAULTCOLOR;

	static void RenderVisuals();

	void AddData(GraphicsObjectWireFrame* go, const Matrix& mat, const Vect& color);

	visualData* GetNewData();

	void RecycleData(visualData* target);
public:

	

	static void Initialize();

	static void ShowAABB(const Vect& minPoint, const Vect& maxPoint, const Vect& color = DEFAULTCOLOR);
	static void ShowOBB(const Vect& minPoint, const Vect& maxPoint, const Matrix& worldMat, const Vect& color = DEFAULTCOLOR);
	static void ShowSphere(const Vect& center, float radius, const Vect& color = DEFAULTCOLOR);
	static void ShowPoint(const Vect& pos, const Vect& color = DEFAULTCOLOR, float markRadius = 1);



};
#endif DEBUGVISUALIZER_H