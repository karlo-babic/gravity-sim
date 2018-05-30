const float PI = 3.141592653;


const int SMOOTH = 30;
void drawCircle(float xc, float yc, float radius)
{
    for (int i = 0; i < SMOOTH; i++)
    {
        float angle = 2 * PI * i / SMOOTH;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        glVertex2f(x + xc, y + yc);
    }
}
