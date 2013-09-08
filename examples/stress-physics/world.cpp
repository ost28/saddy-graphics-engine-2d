#include <p2d/app/object.h>
#include <orthocamera.h>
#include <texturemanager.h>
#include <input.h>
#include <extra/background.h>
#include <p2d/elasticforce.h>

#include "world.h"

#include "gridnode.h"
#include "gridnodedge.h"

#include "bouncesolver.h"

World::World()
{
	this->initApp();
	/*m_world->setDetector(new p2d::MultisamplingCollisionDetector(3))*/;
	m_walls = new p2d::Walls();
	m_find = new p2d::FindContactPoints();
	m_solver = new BounceSolver();
}


World::~World()
{
	delete m_walls;
	delete m_find;
	delete m_solver;
}




void World::run()
{
	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sc->add(new sad::Background("background"));
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("Physics stress test");

	sad::Renderer::ref()->controls()->setMouseMoveHandler( new sad::MethodEventHandler<sad::Event, World>(this, &World::onMouseMove));

	// Load textures to videocard memory and build mipmaps
	sad::Renderer::ref()->textures()->buildAll();
	SL_MESSAGE("Mips built successfully");	
	
	// Set window size to be fixed
	sad::Renderer::ref()->toggleFixedOn();

	m_steptask = new p2d::WorldStepTask(m_world);
	sad::Input::ref()->addPostRenderTask(m_steptask);


	// SETUP WORLD CALLBACKS HERE!!!
	m_world->addHandler(this, &World::onWallNode);
	m_world->addHandler(this, &World::onNodeNode);

	// Add walls
	hst::vector<p2d::Body *> bodies = m_walls->bodies();
	for(int i = 0; i < bodies.count(); i++)
	{
		m_world->add(bodies[i]);
	}

	// Build elastic grid with 9 nodes

	// Produce a nodes of grids
	GridNode * g[9];
	for(int i = 0; i < 9; i++)
	{
		g[i] = new GridNode();
		g[i]->setPosition(p2d::Point(300 + (i % 3) * 100, 400 - (i / 3) * 100));
		// Add a gravity force
		if (i != 0 && i != 2) 
		{
			g[i]->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -60) ) );
		}
		else
		{
			// Make bodies unmovable
			g[i]->body()->weight().setValue(1E+6);
		}
	}

	// Sets forces, between nodes	
	int forces[20][2] =  {
		{0, 1}, {2, 1}, {4, 1},
		{0, 3}, {4, 3}, {6, 3},
		{3, 4}, {1, 4}, {5, 4}, {7, 4},
		{2, 5}, {4, 5}, {8, 5},
		{3, 6}, {7, 6},
		{6, 7}, {4, 7}, {8, 7},
		{5, 8}, {7, 8}
	};
	for(int i = 0; i < 20; i++)
	{
		int  f = forces[i][0];
		int  s = forces[i][1];
		g[s]->addTangentialForce( new p2d::ElasticForce(g[f]->body(), g[s]->body()) );
	}
	int graphic[12][2] = { 
		{0, 1}, {1, 2}, {0, 3}, {1, 4}, {3, 4}, {2, 5},
		{4, 5}, {3, 6}, {4, 7}, {6, 7}, {5, 8}, {7, 8}
	};
	// Build grafical representaion for elastic grid
	for(int i = 0; i < 12; i++)
	{
		sc->add( new GridNodeEdge(g[graphic[i][0]]->body(), g[graphic[i][1]]->body()) );
	}
    
	// Add elastic grid nodes to scene
	for(int i = 0 ; i < 9; i++)
	{
		this->addObject(g[i]);
	}

	// Add ball to scene
	GridNode * ball = new GridNode();
	ball->setPosition(p2d::Point(20, 300));
	ball->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -30) ) );
	ball->body()->setCurrentTangentialVelocity(p2d::Vector(140, 120));
	this->addObject(ball);
	
	// Add FPS counter
	FormattedLabel * label = new FormattedLabel();
	label->setFont("times_lg");
	label->setPoint(0, sad::Renderer::ref()->settings().height());
	label->setFormatString("{0}");
	label->setUpdateInterval(1000.0);
	label->argFPS();
	sc->add( label );


	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}


void World::quit()
{
	sad::Renderer::ref()->quit();
}


/*! Handled this type of collision with manual from
	http://alexandr4784.narod.ru/sdvmpdf1/smgl04_28.pdf
 */
void World::onWallNode(const p2d::CollisionEvent<GridNode, p2d::Wall> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}



void World::onNodeNode(const p2d::CollisionEvent<GridNode, GridNode> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}


void World::onMouseMove(const sad::Event & ev)
{
	//SL_DEBUG(fmt::Format("{0} {1}") << ev.x << ev.y);
}

