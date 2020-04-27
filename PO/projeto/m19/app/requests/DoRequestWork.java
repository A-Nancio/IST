package m19.app.requests;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Form;
import m19.app.exception.RuleFailedException;

/**
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

  Input<Integer> _userId;
  Input<Integer> _workId;
  Input<String> _requestNotification;

  /**
   * @param receiver
   */
  public DoRequestWork(LibraryManager receiver) {
    super(Label.REQUEST_WORK, receiver);
    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try{
      int deadline = _receiver.makeRequest(_userId.value(), _workId.value());
      _display.addLine(Message.workReturnDay(_workId.value(), deadline));
    }
    catch(RuleFailedException rfEx){
      if (rfEx.getRuleIndex() == 3){
        
        Form tempForm = new Form();
        _requestNotification = tempForm.addStringInput(Message.requestReturnNotificationPreference());
        while(true){
          tempForm.parse();
          if (_requestNotification.value().equals("s")){
            _receiver.becomeNotified(_userId.value(), _workId.value());
            break;
          }
          else if (_requestNotification.value().equals("n")){
            break;
          }
        }
      }
      else throw rfEx;
    }
    finally{
      _display.display();
    }
  }
}
