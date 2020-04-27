package m19.core;

import java.io.*;
import m19.core.exception.*;
import m19.app.exception.*;

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
    return _library.getTime();
  }

  public String getUser(int userId) throws NoSuchUserException{
    return _library.getUserDescription(userId);
  }

  public String getUsers(){
    return _library.getSortedUsers();
  }

  public String getWork(int workId) throws NoSuchWorkException{
    return _library.getWorkDescription(workId);
  }

  public String getWorks(){
    return _library.getSortedWorks();
  }

  public String getNotifications(int userId) throws NoSuchUserException{
    return _library.getNotifications(userId);
  }

  public int getUnpaidFine(int userId) throws NoSuchUserException{
    return _library.getUnpaidFine(userId);
  }

  public String keyWordSearch(String term){
    return _library.keyWordSearch(term.toLowerCase());
  }

  public int registerUser(String userName, String email) throws UserRegistrationFailedException{
    if (userName.isEmpty() || email.isEmpty()){
      throw new UserRegistrationFailedException(userName, email);
    }
    return _library.newUser(userName, email);
  }

  public int makeRequest(int userId, int workId) throws RuleFailedException, NoSuchUserException, NoSuchWorkException{
    return _library.makeRequest(userId, workId);
  }

  public int returnWork(int userId, int workId) throws WorkNotBorrowedByUserException, NoSuchUserException, NoSuchWorkException {
    return _library.returnWork(userId, workId);
  }

  public void payFine(int userId) throws NoSuchUserException, UserIsActiveException{
    _library.payOffFine(userId);
  }

  public void addUnpaidFine(int userId, int fine) throws NoSuchUserException{
    _library.addUnpaidFine(userId, fine);
  }

  public void advanceDays(int nDays){
    _library.advanceDays(nDays);
  }

  public void becomeNotified(int userId, int workId) throws NoSuchUserException, NoSuchWorkException{
    _library.becomeNotified(userId, workId);
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
