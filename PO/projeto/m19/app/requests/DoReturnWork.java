package m19.app.requests;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Form;

/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

  Input<Integer> _userId;
  Input<Integer> _workId;
  Input<String> _paymentChoice;

  /**
   * @param receiver
   */
  public DoReturnWork(LibraryManager receiver) {
    super(Label.RETURN_WORK, receiver);
    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    int userId = _userId.value();
    int returnFine = _receiver.returnWork(userId, _workId.value());
    if (returnFine != 0){
      _display.addLine(Message.showFine(userId, _receiver.getUnpaidFine(userId) + returnFine));
      _display.display();

      Form tempForm = new Form();
      _paymentChoice = tempForm.addStringInput(Message.requestFinePaymentChoice());
      
      while(true){
        tempForm.parse();
        if (_paymentChoice.value().equals("s")) {
          _receiver.payFine(userId);
          break;
        }
        else if (_paymentChoice.value().equals("n")){
          _receiver.addUnpaidFine(userId, returnFine);
          break;
        }
      }
    }
  }

}
