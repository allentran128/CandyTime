/* Allen Tran
 *
 * Description
 * Connects the control flow from Gtk to Server
 */
#include "jansson.h"
#include "serverMessage.h"

extern "C" void passOnControl(void * serverPtr, json_t *root) {
  ((Server *)serverPtr)->sendMessageToClient(root);
}
