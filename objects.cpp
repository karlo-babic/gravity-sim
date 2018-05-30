float scale;
int initialSpin;
bool collisionON;
bool fragment;
int trace;

int loop;



class Obj
{
public:
  float position_x, position_y;
  float new_position_x, new_position_y;
  float prevPosition_x, prevPosition_y;
  float radius;
  float mass;
  float tracePoints[60];
  int sizeofTrace = trace * 2;
  int firstTracePoint;

  Obj(float position_x, float position_y, float prevPosition_x, float prevPosition_y, float radius);
  Obj(int n, int d);

  void calcMass();
  void calcGravityPosition(Obj &o);
  void calcInertPosition();
  int collision(Obj &o);
  bool outOfBounds();
  void makeTrace();
  void drawTrace();
};



Obj::Obj(float position_x, float position_y, float prevPosition_x, float prevPosition_y, float radius)
{
  this->position_x = position_x;
  this->position_y = position_y;
  this->radius = radius;
  this->prevPosition_x = prevPosition_x;
  this->prevPosition_y = prevPosition_y;

  for (int i=0; i<60; i++) tracePoints[i] = 1000000;
  tracePoints[0] = position_x;
  tracePoints[1] = position_y;
  firstTracePoint = 0;

  calcMass();
}

Obj::Obj(int n, int d)
{
  position_x = (rand() % 1000 - 500) / 10. * d;
  position_y = (rand() % 1000 - 500) / 10. * d;

  radius = (rand() % n) / 10. + 0.2;

  if (initialSpin)
    {
      float xydistance = sqrt(position_x*position_x + position_y*position_y);
      float xratio = position_x / xydistance;
      float angle = acos(xratio);
      if (position_y < 0) angle = - angle;

      angle -= (100) / (xydistance*xydistance);
      float positionBack_x = position_x - xydistance * cos(angle);
      float positionBack_y = position_y - xydistance * sin(angle);
      prevPosition_x = position_x + positionBack_x * initialSpin;
      prevPosition_y = position_y + positionBack_y * initialSpin;
    }
  else
    {
      prevPosition_x = position_x;
      prevPosition_y = position_y;
    }

  for (int i=0; i<60; i++) tracePoints[i] = 1000000;
  tracePoints[0] = position_x;
  tracePoints[1] = position_y;
  firstTracePoint = 0;

  calcMass();
}


void Obj::calcMass()
{
  mass = 20.94395 * pow(radius,3);    // 4/3. * 3.14 * pow(radius,3) * 5
}


void Obj::calcGravityPosition(Obj &o)
{
  float xDistance = position_x - o.position_x;
  float yDistance = position_y - o.position_y;

  float relXDistance = xDistance;
  float relYDistance = yDistance;

  if (xDistance < 0) xDistance = -xDistance;
  if (yDistance < 0) yDistance = -yDistance;

  float distance = sqrt(pow(xDistance,2) + pow(yDistance,2));
  if (distance == 0) distance = 0.1;
  float forceGravity = (mass * o.mass) / pow(distance,2);

  float xy_Distance = xDistance + yDistance;
  if (xy_Distance == 0) xy_Distance = 0.1;

  float xForce = relXDistance / xy_Distance * forceGravity;
  float yForce = relYDistance / xy_Distance * forceGravity;

  new_position_x -= xForce / (mass/2);
  new_position_y -= yForce / (mass/2);
  o.new_position_x += xForce / (o.mass/2);
  o.new_position_y += yForce / (o.mass/2);
}


void Obj::calcInertPosition()
{
  new_position_x += position_x - prevPosition_x;
  new_position_y += position_y - prevPosition_y;
}


int Obj::collision(Obj &o)
{
  bool skipped = false;
  float skip_relx1 = position_x - o.position_x;
  float skip_relx2 = new_position_x - o.new_position_x;
  float skip_rely1 = position_y - o.position_y;
  float skip_rely2 = new_position_y - o.new_position_y;
  if (skip_relx1 * skip_relx2 <= 0 && skip_rely1 * skip_rely2 <= 0) skipped = true;

  float xDistance = new_position_x - o.new_position_x;
  float yDistance = new_position_y - o.new_position_y;
  if (xDistance < 0) xDistance = -xDistance;
  if (yDistance < 0) yDistance = -yDistance;
  float distance = sqrt(pow(xDistance,2) + pow(yDistance,2));

  if (distance < radius + o.radius || skipped)
    {
      if (fragment)
	{
	  o.mass /= 2.;
	  o.radius = pow(o.mass/20.94395, 1/3.);
	}

      radius = pow((pow(radius,3) + pow(o.radius,3)), 1/3.);

      float diffX = new_position_x - o.new_position_x;
      float diffY = new_position_y - o.new_position_y;
      o.position_x += diffX;
      o.position_y += diffY;

      float massRatio = o.mass / mass;
      float forceX = (position_x - o.position_x) * massRatio;
      float forceY = (position_y - o.position_y) * massRatio;
      if (fragment)
	{
	  forceX += forceX/2;
	  forceY += forceY/2;
	}
      position_x = position_x - forceX;
      position_y = position_y - forceY;

      calcMass();

      return 1;
    }

  return 0;
}


bool Obj::outOfBounds()
{
  float x, y;

  if (new_position_x < 0) x = - new_position_x;
  else x = new_position_x;
  if (new_position_y < 0) y = - new_position_y;
  else y = new_position_y;

  if (x > 50 * scale * 4 || y > 50 * scale * 4) return true;
  else return false;
}


void Obj::makeTrace()
{
  int i;
  int j;
  for (i=0; i<sizeofTrace-2; i++)
    {
      j = i;
      if (j == sizeofTrace - 2) j = 0;
      else j += 2;
      if (tracePoints[i] == prevPosition_x && tracePoints[i+1] == prevPosition_y && !(tracePoints[j] == prevPosition_x && tracePoints[j+1] == prevPosition_y))
	{
	  break;
	}
      i++;
    }
  if (i == sizeofTrace - 2) i = 0;
  else i += 2;

  tracePoints[i] = position_x;
  tracePoints[i+1] = position_y;
  firstTracePoint = i;
}


void Obj::drawTrace()
{
  int i = firstTracePoint;
  do
    {
      if (tracePoints[i] == 1000000) break;

      glVertex2f(tracePoints[i] / scale, tracePoints[i+1] / scale);

      i -= 2;
      if (i == -2) i = sizeofTrace-2;
    } while (i != firstTracePoint);
}
