#ifndef ACTION_H
#define ACTION_H

#include "BehaviourTree.h"
#include "Entity.h"
#include "Component.h"
#include "PositionComponent.h"
#include "AIComponent.h"
#include "AiInputComponent.h"
#include "PlayerPhysicsComponent.h"

/// <summary>
/// Action base class. All other concrete action
/// classes are derived from this, and their function
/// overridden from this.
/// Pattern resembles command pattern.
/// </summary>
class Action : public BehaviourTree::Node
{
public:
	Action(Entity * e, AiInputComponent * a) :
	m_entity(e),
	m_input(a)
	{}
	//Run function called by the behaviour tree, overridden by the concrete actions
	virtual bool run() = 0; 
	//Stores a reference to the entity that owns the AI component
	Entity * m_entity; 
	//Stores a reference to the AIinput component for moving the AI player
	AiInputComponent * m_input; 
};

#endif

#ifndef WALKLEFTACTION_H
#define WALKLEFTACTION_H

class WalkLeftAction : public Action
{
public:
	WalkLeftAction(Entity * e, AiInputComponent * a) :
		Action(e, a)
	{
	}

	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called moves the player left
	/// by calling the stickleft command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		std::cout << "MOVE LEFT" << std::endl;
		//Passes STICKLEFT command to the AI input handler
		//m_input->handleInput("STICKLEFT", m_entity);
		m_input->m_current["STICKLEFT"] = true;
		//Return true to maintain behaviour tree flow of execution
		return true;
	}
};

#endif

#ifndef WALKRIGHTACTION_H
#define WALKRIGHTACTION_H

class WalkRightAction : public Action
{
public:
	WalkRightAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called moves the player right
	/// by calling the stickright command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Passes STICKRIGHT command to the AI input handler
		//m_input->handleInput("STICKRIGHT", m_entity);
		m_input->m_current["STICKRIGHT"] = true;
		//return function as true to keep tree runnning
		return true;
	}
};

#endif

#ifndef JUMPACTION_H
#define JUMPACTION_H

class JumpAction : public Action
{
public:
	JumpAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}
	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called makes the AI player jump
	/// by calling the YBTN command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Passes YBTN command to AI input handler
		//m_input->handleInput("YBTN", m_entity);
		m_input->m_current["YBTN"] = true;
		std::cout << "JUMP" << std::endl;
		return true;
	}
};

#endif

#ifndef PUNCHACTION_H
#define PUNCHACTION_H

class PunchAction : public Action
{
public:
	PunchAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Makes the AI player jump by passing the XBTN command
	/// to the AI input handler object.
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//m_input->handleInput("XBTN", m_entity);
		m_input->m_current["XBTN"] = true;
		return true;
	}
};

#endif

#ifndef CHECKNEAREST_H
#define CHECKNEAREST_H

class CheckNearest : public Action
{
public:
	CheckNearest(std::vector<Entity *>* e, Entity * s, AiInputComponent * a) :
		m_entities(e), Action(s, a)
	{
	}

	/// <summary>
	/// Behaviour tree run function overidden from Action base class.
	/// Loops through vector of all players in the current game and 
	/// finds the one nearest to the current AI entity.
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Container to store nearest entity
		auto nearest = new Entity("temp");

		//If entities vector is not empty, set nearest to first element
		if (!m_entities->empty())
		{
			nearest = m_entities->at(0);

			//Get position component of current AI entity
			auto self_pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
			//Get position component from entity
			auto otherPos = dynamic_cast<PositionComponent *>(&nearest->getComponent("Pos"));

			//Loop through all entities
			for (auto entity : *m_entities)
			{
				if (entity->m_ID != "AI")
				{
					//Get position component of the current entity in the loop
					auto newPos = dynamic_cast<PositionComponent *>(&entity->getComponent("Pos"));
					//Check distance between self and previous position, and self and new position
					if (dist(newPos->position, self_pos->position) < dist(otherPos->position, self_pos->position))
					{
						//If new position is closer, the entity it belongs to is assigned to nearest
						nearest = entity;
					}
				}
			}
			//Cast AI component
			auto e = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
			//Assign nearest player to variable in the AI 
			e->nearestPlayer = nearest;
			return true;
		}
		return false;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
	std::vector<Entity *>* m_entities;
};
#endif

#ifndef CLOSEENOUGH_H
#define CLOSEENOUGH_H

class CloseEnough : public Action
{
public:
	CloseEnough(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));
		//Return true if dist between two entities is less than 100
		return dist(pos->position, nearest->position) < 50 ? true : false;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKPLAYERDIRECTION_H
#define CHECKPLAYERDIRECTION_H

class CheckPlayerDirection : public Action
{
public:
	CheckPlayerDirection(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overidden from the Action base class
	/// casts a vector from the AI entity to the nearest
	/// player as one of 4 directions
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//Check if the nearest player is to the left of AI
		if (nearest->position.x < pos->position.x)
		{
			//move left
			//m_input->handleInput("STICKLEFT", m_entity);
			m_input->m_current["STICKLEFT"] = true;
		} //Check if the nearest player is to the right of AI
		else if (nearest->position.x > pos->position.x)
		{
			//move right
			//m_input->handleInput("STICKRIGHT", m_entity);
			m_input->m_current["STICKRIGHT"] = true;
		}
		else //If player and AI are on the same X coord, jump
		{
			//jump
			//m_input->handleInput("YBTN", m_entity);
			m_input->m_current["YBTN"] = true;
		}
		//Return function as true to continue tree iteration
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKHEALTH_H
#define CHECKHEALTH_H

class CheckHealth : public Action
{
public:
	CheckHealth(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overidden from the Action base class function.
	/// Down casts the self entity component to a play physics component.
	/// Returns true if the current damage percentage is greater than 50
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		auto p = dynamic_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));
		return p->damagePercentage() > 50 ? true : false;
	}
};

#endif

#ifndef FLEEACTION_H
#define FLEEACTION_H

class FleeAction : public Action
{
public:
	FleeAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Position"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//While the distance to the nearest enemy is less than 200
		while (dist(pos->position, nearest->position) < 200)
		{
			//If they're left of AI, move right
			if (nearest->position.x < pos->position.x)
			{
				//m_input->handleInput("STICKRIGHT", m_entity);
				m_input->m_current["STICKRIGHT"] = true;
			}
			else //otherwise move left
			{
				//m_input->handleInput("STICKLEFT", m_entity);
				m_input->m_current["STICKLEFT"] = true;
			}
		}
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKABOVE_H
#define CHECKABOVE_H

class CheckAbove : public Action
{
public:
	CheckAbove(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//Return function true if player position is below(screen axis) the AI 
		return nearest->position.y > pos->position.y - 50 ? true : false;
	}
};

#endif

#ifndef CHECKPLAYERHEALTH_H
#define CHECKPLAYERHEALTH_H

class CheckPlayerHealth : public Action
{
public:
	CheckPlayerHealth(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	/// <summary>
	/// Run function overidden from Action base class.
	/// Checks if the nearest players damage percentage
	/// is high enough to attack them
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PlayerPhysicsComponent *>(&comp->nearestPlayer->getComponent("Player Physics"));

		//Return true if nearest players damage % is greater than 50
		return nearest->damagePercentage() > 50 ? true : false;
	}
};

#endif

#ifndef DROPACTION_H
#define DROPACTION_H

class DropAction : public Action
{
public:
	DropAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		//m_input->handleInput("STICKDOWN", m_entity);
		m_input->m_current["STICKDOWN"] = true;
		return true;
	}
};

#endif

#ifndef MOVETOPLAYER_H
#define MOVETOPLAYER_H

class MoveToPlayer : public Action
{
public:
	MoveToPlayer(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//If the AI is more than 50 pixels from the player
		if (dist(nearest->position, pos->position) > 50)
		{
			//If AI is right of player
			if (nearest->position.x < pos->position.x)
			{
				if (comp->onEdge)
				{
					m_input->m_current["YBTN"];
					comp->onEdge = false;
				}
				//Move left
				m_input->m_current["STICKRIGHT"] = false;
				m_input->m_current["STICKLEFT"] = true;
			}
			else //AI is left of player
			{
				if (comp->onEdge)
				{
					m_input->m_current["YBTN"];
					comp->onEdge = false;
				}
				//Move right
				m_input->m_current["STICKLEFT"] = false;
				m_input->m_current["STICKRIGHT"] = true;
			}
		}
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif