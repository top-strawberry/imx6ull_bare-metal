#include "usr_app.h"

extern TOP_INFO usr_app;

extern int8_t usr_app_init(TOP_INFO *self);

int main(void)
{
	usr_app_init(&usr_app);
    while(1) {
		usr_app.usr_app_run(&usr_app);
    }

    return 0;
}





