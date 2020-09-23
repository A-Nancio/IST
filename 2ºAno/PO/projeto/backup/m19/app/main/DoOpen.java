  package m19.app.main;

import m19.app.exception.FileOpenFailedException;
import m19.core.LibraryManager;
import m19.core.exception.MissingFileAssociationException;
import m19.core.exception.FailedToOpenFileException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * 4.1.1. Open existing document.
 */
public class DoOpen extends Command<LibraryManager> {

  Input<String> _file;

  /**
   * @param receiver
   */
  public DoOpen(LibraryManager receiver) {
    super(Label.OPEN, receiver);
    _file = _form.addStringInput(Message.openFile());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    try {
      _form.parse();
      _receiver.loadFile(_file.value());
    }
    catch (FailedToOpenFileException ftofEx) {
      throw new FileOpenFailedException(ftofEx.getName());
    }
    catch (IOException ioEx) {
      ioEx.printStackTrace();
    }
  }
}
