#include "Sequence.h"

class Title : public Sequence
{
public:
    Title();
    ~Title() override;

    void input() override;
    void update() override;
    void draw() override;
    Sequence* nextSequence() override;

private:
    Sequence* mNext = nullptr;
};
