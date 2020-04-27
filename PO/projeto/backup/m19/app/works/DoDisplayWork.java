package m19.app.works;

import m19.core.LibraryManager;

import m19.app.exception.NoSuchWorkException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
// FIXME import other core concepts
// FIXME import ui concepts

/**
 * 4.3.1. Display work.
 */
public class DoDisplayWork extends Command<LibraryManager> {

  Input<Integer> _id;

  /**
   * @param receiver
   */
  public DoDisplayWork(LibraryManager receiver) {
    super(Label.SHOW_WORK, receiver);
    _id = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    _display.addLine(_receiver.getWork(_id.value()));
    _display.display();
  }
}
