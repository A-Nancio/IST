package m19.app.users;

import m19.app.exception.UserRegistrationFailedException;
import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
// FIXME import other core concepts
// FIXME import other ui concepts

/**
 * 4.2.1. Register new user.
 */
public class DoRegisterUser extends Command<LibraryManager> {

  // FIXME define input fields
  private Input<String> _name;
  private Input<String> _email;
  /**
   * @param receiver
   */
  public DoRegisterUser(LibraryManager receiver) {
    super(Label.REGISTER_USER, receiver);
    _name = _form.addStringInput(Message.requestUserName());
    _email = _form.addStringInput(Message.requestUserEmail());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws UserRegistrationFailedException {
    _form.parse();
    int idUser = _receiver.registerUser(_name.value(), _email.value());
    _display.addLine(Message.userRegistrationSuccessful(idUser));
    _display.display();
  }
}
