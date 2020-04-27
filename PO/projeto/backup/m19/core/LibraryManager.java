package m19.core;

import java.io.*;

import m19.core.exception.MissingFileAssociationException;
import m19.core.exception.BadEntrySpecificationException;
import m19.core.exception.ImportFileException;
import m19.core.exception.FailedToOpenFileException;
import m19.core.user.User;
import m19.core.work.Work;

import m19.app.exception.*;

import java.util.*;

/**
 * The façade class.
 */
public class LibraryManager {

  private Library _library;
  private String _associatedFileName;
  private Parser _parser;

  private String _filename;

  public LibraryManager(){
    _library = new Library();
    _parser = new Parser(_library);
  }

  public String getAssociatedFileName(){
    return _associatedFileName;
  }

  public int getCurrentDate(){
    Date time = _library.getTime();
    return time.getCurrentDate();
  }

  public String getUser(int userId) throws NoSuchUserException{
    User user = _library.getUser(userId);
    return user.toString();
  }

  public String getUsers(){
    List<User> sortedUsers = _library.getSortedUsers();
    String ret = "";
    for (User iter: sortedUsers){
      ret += iter.toString() + "\n";
    }
    return ret;
  }

  public String getWork(int workId) throws NoSuchWorkException{
    Work work = _library.getWork(workId);
    return work.toString();
  }

  public String getWorks(){
    List<Work> sortedWorks = _library.getSortedWorks();
    String ret = "";
    for (Work iter: sortedWorks){
      ret += iter.toString() + "\n";
    }
    return ret;
  }

  public int registerUser(String userName, String email) throws UserRegistrationFailedException{
    if (userName.isEmpty() || email.isEmpty()){
      throw new UserRegistrationFailedException(userName, email);
    }
    return _library.newUser(userName, email);
  }

  public void advanceDays(int nDays){
    Date time = _library.getTime();
    time.advanceDay(nDays);
  }

  public void getNotifications(int userId) throws NoSuchUserException {
    List<Notification> _notifications = library
  }

  /**
   * Serialize the persistent state of this application.
   * 
   * @throws MissingFileAssociationException if the name of the file to store the persistent
   *         state has not been set yet.
   * @throws IOException if some error happen during the serialization of the persistent state
   *
   */
  public void save() throws MissingFileAssociationException {
    if (_associatedFileName == null) throw new MissingFileAssociationException();
    try (FileOutputStream file = new FileOutputStream(_associatedFileName);
      ObjectOutputStream out = new ObjectOutputStream(file)) {
      out.writeObject(_library);
    }
    catch(IOException ioEx){
      ioEx.printStackTrace();
    }
  }

  /**
   * Serialize the persistent state of this application into the specified file.
   * 
   * @param filename the name of the target file
   *
   * @throws MissingFileAssociationException if the name of the file to store the persistent
   *         is not a valid one.
   * @throws IOException if some error happen during the serialization of the persistent state
   */
  public void saveAs(String filename) throws MissingFileAssociationException {
    _associatedFileName = filename;
    this.save();
  }

  /**
   * Recover the previously serialized persitent state of this application.
   * 
   * @param filename the name of the file containing the perssitente state to recover
   *
   * @throws IOException if there is a reading error while processing the file
   * @throws FileNotFoundException if the file does not exist
   * @throws ClassNotFoundException 
   */
  public void loadFile(String filename) throws FailedToOpenFileException, IOException {
    try(FileInputStream inputFile = new FileInputStream(filename);
      ObjectInputStream in = new ObjectInputStream(inputFile)){
      _library = (Library)in.readObject();
      _associatedFileName = filename;
      in.close();
      inputFile.close();
    }
    catch (FileNotFoundException | ClassNotFoundException cnfEx) {
      throw new FailedToOpenFileException(filename);
    }
  }
  
  /**
   * Set the state of this application from a textual representation stored into a file.
   * 
   * @param datafile the filename of the file with the textual represntation of the state of this application.
   * @throws ImportFileException if it happens some error during the parsing of the textual representation.
   */
  public void importFile(String datafile) throws ImportFileException {
    try {
      _parser.parseFile(datafile);
    } catch (IOException | BadEntrySpecificationException e) {
      throw new ImportFileException (e);
    }
  }
}
