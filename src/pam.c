#include <string.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>

#include "touch.h"

#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#define PIN "1612"

// TODO what do the other functions do?
// TODO are they required?

PAM_EXTERN int pam_sm_open_session(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{ return PAM_SUCCESS; }

PAM_EXTERN int pam_sm_close_session(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{ return PAM_SUCCESS; }

PAM_EXTERN int pam_sm_acct_mgmt(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{ return PAM_SUCCESS; }

PAM_EXTERN int pam_sm_authenticate(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{

    int pin_len = strlen(PIN);

    touch_start(pin_len);
    while (!touch_finished());

    char *pin = touch_pin();
    int ret = (strncmp(pin, PIN, pin_len) == 0)
        ? PAM_SUCCESS : PAM_AUTH_ERR;

    touch_stop();

    return ret;

}

PAM_EXTERN int pam_sm_setcred(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{ return PAM_SUCCESS; }

PAM_EXTERN int pam_sm_chauthtok(
  pam_handle_t *pamh, int flags, int argc, const char **argv)
{ return PAM_SUCCESS; }
