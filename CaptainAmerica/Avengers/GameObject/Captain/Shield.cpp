#include "Shield.h"
#include "../../GameFramework/Map/Grid.h"

Shield * Shield::__instance = NULL;

Shield * Shield::GetInstance()
{
	if (__instance == NULL)
		__instance = new Shield();
	return __instance;
}

Shield::Shield()
{
	LoadResources();

	//IsActive = false;

	width = 16;
	height = 16;

	vx = 0.125f;

	collider.x = x;
	collider.y = y;
	collider.vx = 0;
	collider.vy = 0;
	collider.width = 16;
	collider.height = 16;

	maxDistance = SCREEN_WIDTH / 3;

	state = SHIELD_LEFT;
}
//void Shuriken::CreateShuriken(float posx, float posy, float dt, bool isLeft)
//{
//	this->Active = true;
//	this->x = posx;
//	this->y = posy;
//
//	float vx = SHURIKEN_SPEED * (isLeft == true ? -1 : 1);
//	this->vx = vx;
//	this->collider.vx = vx;
//
//	collider.x = posx;
//	collider.y = posy;
//	this->dt = dt;
//}

void Shield::LoadResources()
{
	LoadTXT loadTXT;
	RECT* listSprite = loadTXT.LoadRect((char*)"Resources\\Captain\\CharacterSprite.txt");
	Animation * anim = new Animation(100);
	//left
	Sprite * sprite = new Sprite(CAPTAIN_TEXTURE_LOCATION, listSprite[46], CAPTAIN_TEXTURE_TRANS_COLOR);
	anim->AddFrame(sprite);
	animations.push_back(anim);
	//center
	anim = new Animation(100);
	sprite = new Sprite(CAPTAIN_TEXTURE_LOCATION, listSprite[47], CAPTAIN_TEXTURE_TRANS_COLOR);
	anim->AddFrame(sprite);
	animations.push_back(anim);
	//top
	anim = new Animation(100);
	sprite = new Sprite(CAPTAIN_TEXTURE_LOCATION, listSprite[48], CAPTAIN_TEXTURE_TRANS_COLOR);
	anim->AddFrame(sprite);
	animations.push_back(anim);
	//down
	anim = new Animation(100);
	sprite = new Sprite(CAPTAIN_TEXTURE_LOCATION, listSprite[49], CAPTAIN_TEXTURE_TRANS_COLOR);
	anim->AddFrame(sprite);
	animations.push_back(anim);

	anim = new Animation(100);
	RECT r;
	r.left = r.top = r.bottom = r.right = 0;
	sprite = new Sprite(CAPTAIN_TEXTURE_LOCATION, r, CAPTAIN_TEXTURE_TRANS_COLOR);
	anim->AddFrame(sprite);
	animations.push_back(anim);
}

void Shield::Update(DWORD dt)
{
	this->SetSpeedY(0);
	Captain* captain = Captain::GetInstance();
	if (captain->isThrowing) {
		this->state = SHIELD_UP;
		SheildAttacking();
	}
	else
	{
		if (captain->GetStateNum() == CAPTAIN_ANI_IDLE || captain->GetStateNum() == CAPTAIN_ANI_WALK)
		{
			this->state = SHIELD_LEFT;
			this->SetSpeedX(0);
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX() - 2);
			else
				this->SetPositionX(captain->GetPositionX() + 12);
			this->SetPositionY(captain->GetPositionY() - 8);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_JUMP)
		{
			this->SetSpeedX(0);
			this->state = NO_SHIELD;
			if (captain->GetSpeedY() >= 0.25)
				this->state = NO_SHIELD;
			else
			{
				this->state = SHIELD_CENTER;
			}
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX());
			else
				this->SetPositionX(captain->GetPositionX() + 8);
			this->SetPositionY(captain->GetPositionY() - 4);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_CROUCH)
		{
			this->state = SHIELD_LEFT;
			this->SetSpeedX(0);
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX() - 2);
			else
				this->SetPositionX(captain->GetPositionX() + 12);
			this->SetPositionY(captain->GetPositionY() - 26);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_SHIELD_UP)
		{
			this->state = SHIELD_UP;
			this->SetSpeedX(0);
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX());
			else
				this->SetPositionX(captain->GetPositionX() + 8);
			this->SetPositionY(captain->GetPositionY() + 2);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_KICK)
		{
			this->state = SHIELD_LEFT;
			this->SetSpeedX(0);
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX() + 28);
			else
				this->SetPositionX(captain->GetPositionX() - 18);
			this->SetPositionY(captain->GetPositionY() - 5);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_CROUCH_HIT)
		{
			this->state = SHIELD_CENTER;
			this->SetSpeedX(0);
			if (captain->IsLeft())
				this->SetPositionX(captain->GetPositionX() +15);
			else
				this->SetPositionX(captain->GetPositionX()-3);
			this->SetPositionY(captain->GetPositionY() - 25);
		}
		if (captain->GetStateNum() == CAPTAIN_ANI_WADE)
			this->state = NO_SHIELD;
	}
	this->SetPositionX((float)(this->GetPositionX() + this->GetSpeedX()* dt*(isLeft == true ? -1 : 1)));
	this->SetPositionY((float)(this->GetPositionY() + this->GetSpeedY()* dt));
}

void Shield::SheildAttacking() 
{
	Captain* captain = Captain::GetInstance();
	if (isStart)
	{
		this->isCaptainLeft = captain->IsLeft();
		if (captain->IsLeft())
			this->SetPositionX(captain->GetPositionX() + 20);
		else
			this->SetPositionX(captain->GetPositionX() - 14);
		this->distance = captain->GetPositionX() + maxDistance * (isCaptainLeft ? -1 : 1);
		this->SetSpeedX(SHIELD_SPEED * 1 * (isCaptainLeft ? -1 : 1));
		this->SetPositionY(captain->GetPositionY() - 6);

		this->isStart = false;
	}
	else
	{

		int cpos = captain->GetPositionY() - 20;
		if ((isCaptainLeft ? (this->GetPositionX() < this->distance) : (this->GetPositionX() >= this->distance) && isReturn))
		{
			this->SetSpeedX(SHIELD_SPEED * -1 * (isCaptainLeft ? -1 : 1));
			isReturn = false;
		}
		if (abs(this->GetPositionY() - cpos) >= 1)
		{
			if (this->GetPositionY() > cpos)
			{
				float temp = this->GetPositionY() - 1.25;
				this->SetPositionY(temp);
			}
			else
			{
				float temp = this->GetPositionY() + 1.25;
				this->SetPositionY(temp);
			}
		}
		if (!isReturn && abs(this->GetPositionX() - captain->GetPositionX()) <= 30)
		{

			isStart = true;
			isReturn = true;
			captain->isThrowing = false;

		}
	}
}
//
//void Shield::Update(DWORD dt)
//{
//	this->SetSpeedY(0);
//	distance += abs(this->GetSpeedX() * dt);
//	Captain* captain = Captain::GetInstance();
//
//	if (captain->GetIdleState() || captain->GetWalkingState())
//	{
//		this->state = SHIELD_LEFT;
//		this->SetSpeedX(0);
//		if(captain->IsLeft())
//			this->SetPositionX(captain->GetPositionX() - 10);
//		else
//			this->SetPositionX(captain->GetPositionX() + 10);
//		this->SetPositionY(captain->GetPositionY() - 10);
//	}
//	if (captain->GetJumpingState())
//	{
//		this->state = SHIELD_CENTER;
//		this->SetSpeedX(0);
//		this->SetPositionX(captain->GetPositionX());
//		this->SetPositionY(captain->GetPositionY());
//	}
//	if (captain->GetCrouchingState())
//	{
//		this->state = SHIELD_DOWN;
//		this->SetSpeedX(0);
//		this->SetPositionX(captain->GetPositionX());
//		this->SetPositionY(captain->GetPositionY());
//	}
//		
//	/*if (abs(captain->GetPositionX() - this->GetPositionX()) >= 150)
//	{
//		distance = 0;
//		this->SetSpeedX(this->GetSpeedX() * -1);
//	}*/
//	/*if (ninja->GetPositionY() > this->GetPositionY())
//	{
//		this->SetSpeedY(0.125f);
//	}
//	else if (ninja->GetPositionY() < this->GetPositionY())
//	{
//		this->SetSpeedY(-0.125f);
//	}*/
//
//	this->SetPositionX((float)(this->GetPositionX() + this->GetSpeedX()* dt*(isLeft == true ? -1 : 1)));
//	this->SetPositionY((float)(this->GetPositionY() + this->GetSpeedY()* dt));
//
//	//vector<LPCOLLISIONEVENT> coEvents;
//	/*vector<Enemy* > enemies = Grid::GetInstance()->GetAllEnemies();
//	if (Game::GetInstance()->GetStage() == Stage::STAGE_BOSS)
//	{
//		enemies = Grid::GetInstance()->GetAllEnemies();
//	}
//	this->CalcPotentialCollisionsAttackingEnemy(enemies, coEvents);*/
//}
//
void Shield::Render()
{
	Captain * captain = Captain::GetInstance();
	//if (captain->IsThrowing() == true)
	{
		SpriteData spriteEnemyData;

		spriteEnemyData.width = 16 ;
		spriteEnemyData.height = 16 ;
		spriteEnemyData.x = this->GetPositionX();
		spriteEnemyData.y = this->GetPositionY();

		spriteEnemyData.scale = 1;
		spriteEnemyData.angle = 0;
		if (captain->GetStateNum() != CAPTAIN_ANI_KICK) 
		{
			spriteEnemyData.isLeft = captain->IsLeft();
			spriteEnemyData.isFlipped = captain->IsFlipped();
		}
		else
		{
			spriteEnemyData.isLeft = !captain->IsLeft();
			spriteEnemyData.isFlipped = !captain->IsFlipped();
		}

		if (this->state==SHIELD_LEFT)
		{
			this->animations[SHIELD_LEFT]->Render(spriteEnemyData);
		}
		if(this->state==SHIELD_DOWN)
			this->animations[SHIELD_DOWN]->Render(spriteEnemyData);
		if (this->state == SHIELD_CENTER)
			this->animations[SHIELD_CENTER]->Render(spriteEnemyData);
		if(this->state==SHIELD_DOWN)
			this->animations[SHIELD_DOWN]->Render(spriteEnemyData);
		if (this->state == SHIELD_UP)
			this->animations[SHIELD_UP]->Render(spriteEnemyData);
		if (this->state == NO_SHIELD)
			this->animations[NO_SHIELD]->Render(spriteEnemyData);
	}
}

Shield::~Shield()
{
}

