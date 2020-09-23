package m19.app.main;

import m19.core.LibraryManager;
import m19.core.exception.MissingFileAssociationException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

import java.io.IOException;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<LibraryManager> {
  
  Input<String> _fileName;

  /**
   * @param receiver
   */
  public DoSave(LibraryManager receiver) {
    super(Label.SAVE, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException{
    try{
      _receiver.save();
    }
    catch(MissingFileAssociationException mfaEx){
      try{
        _fileName = _form.addStringInput(Message.newSaveAs());
        _form.parse();
        _receiver.saveAs(_fileName.value());
      }
      catch (MissingFileAssociationException ex){
        mfaEx.printStackTrace();
      }
    }
  }
}
