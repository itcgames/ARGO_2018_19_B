#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "Commands.h"


class PlayerInputComponent : public InputComponent {

public:
	PlayerInputComponent() :
		InputComponent()
	{

	}
	void handleInput(void* e) override
	{
		m_currentCMD = nullptr;
		auto entity = static_cast<Entity*>(e);

		if (isButtonPressed("YBTN"))
		{
			m_currentCMD = &m_jumpCMD;
		}
		else if(isButtonPressed("XBTN"))
		{
			if (isButtonHeld("STICKUP"))
				m_currentCMD = &m_uppercutCMD;
			else
				m_currentCMD = &m_punchCMD;
		}
		else if (isButtonHeld("RBBTN") && isButtonHeld("LBBTN"))
		{
			m_currentCMD = &m_superCMD;
		}
		else if (isButtonPressed("ABTN"))
		{
			m_currentCMD = &m_kickCMD;
		}
		else if (isButtonPressed("STICKDOWN"))
		{
			m_currentCMD = &m_phaseDownCMD;
		}
		else if (isButtonHeld("STICKLEFT") || isButtonHeld("STICKDOWNLEFT") || isButtonHeld("STICKUPLEFT"))
		{
			m_currentCMD = &m_moveLeftCMD;
		}
		else if (isButtonHeld("STICKRIGHT") || isButtonHeld("STICKDOWNRIGHT") || isButtonHeld("STICKUPRIGHT"))
		{
			m_currentCMD = &m_moveRightCMD;
		}

		//If the current command was set, execute the command
		if (nullptr != m_currentCMD)
		{
			if ((m_currentCMD == (Command*)&m_moveLeftCMD
			&& m_previousCMD != (Command*)&m_moveLeftCMD)
			|| (m_currentCMD == (Command*)&m_moveRightCMD
			&& m_previousCMD != (Command*)&m_moveRightCMD))
			{
				//Send the online update, as we will be setting the body position manually
			}

			m_currentCMD->execute(*entity);
		}
		else if(static_cast<AttackComponent*>(&entity->getComponent("Attack"))->attackActive() == false)
			static_cast<AnimationComponent*>(&entity->getComponent("Animation"))->playAnimation("Idle", true);

		m_previousCMD = m_currentCMD;
	}

private:
	//Commands
	JumpCommand m_jumpCMD;
	MoveLeftCommand m_moveLeftCMD;
	MoveRightCommand m_moveRightCMD;
	PunchCommand m_punchCMD;
	KickCommand m_kickCMD;
	UppercutCommand m_uppercutCMD;
	PhaseDownCommand m_phaseDownCMD;
	SuperCommand m_superCMD;
	Command* m_currentCMD, *m_previousCMD;
};



#endif