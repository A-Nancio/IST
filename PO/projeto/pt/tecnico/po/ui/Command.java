/* $Id: Command.java,v 1.4 2017/09/05 16:28:29 david Exp $ */
package pt.tecnico.po.ui;

/**
 * Abstract class <code>Command</code> represents an operation that can be
 * carried out by or over an entity.
 * 
 * @param <Receiver>
 *          the entity providing the command's context.
 */
public abstract class Command<Receiver> {

  /** Indicates whether, in a menu, this is the last command. */
  private boolean _last;

  /** The command label in a menu. */
  private String _title;

  /** The entity providing the command's context. */
  final protected Receiver _receiver;

  /** The validity predicate: is the command available/visible? */
  protected ValidityPredicate<Receiver> _validityPredicate = null;

  /** This command's input form. */
  final protected Form _form;

  /** This command's display. */
  final protected Display _display;

  /**
   * @param last
   *          indicates whether, in a menu, this is the last command.
   * @param title
   *          the command label in a menu.
   */
  public Command(boolean last, String title) {
    this(last, title, null);
  }

  /**
   * @param last
   *            indicates whether, in a menu, this is the last command.
   * @param title
   *            the command label in a menu.
   * @param receiver
   *            the entity providing the command's context.
   */
  public Command(boolean last, String title, Receiver receiver) {
    _last = last;
    _title = title;
    _form = new Form(_title);
    _display = new Display(_title);
    _receiver = receiver;
    _validityPredicate = new ValidityPredicate<Receiver>(receiver) {
      @Override
      public boolean isValid() {
        return true;
      }
    };
  }

  /**
   * @param last
   *            indicates whether, in a menu, this is the last command.
   * @param title
   *            the command label in a menu.
   * @param receiver
   *            the entity providing the command's context.
   * @param predicate
   *            custom validity predicate
   */
  public Command(boolean last, String title, Receiver receiver,
      ValidityPredicate<Receiver> predicate) {
    this(last, title, receiver);
    _validityPredicate = predicate;
  }

  /**
   * @param title
   *          the command label in a menu.
   * @param receiver
   *          the entity providing the command's context.
   */
  public Command(String title, Receiver receiver) {
    this(false, title, receiver);
  }

  /**
   * @param title
   *            the command label in a menu.
   * @param receiver
   *            the entity providing the command's context.
   * @param predicate
   *            custom validity predicate
   */
  public Command(String title, Receiver receiver, ValidityPredicate<Receiver> predicate) {
    this(false, title, receiver, predicate);
  }

  /**
   * @return the command's title.
   */
  public final String title() {
    return _title;
  }

  /**
   * Indicates whether, in a menu, this is the last command.
   * 
   * @return true if, in a menu, this is the last command.
   */
  public boolean isLast() {
    return _last;
  }

  /**
   * @return whether the command is valid for the given receiver.
   */
  public boolean isValid() {
    return _validityPredicate.isValid();
  }

  /**
   * @throws DialogException
   */
  public final void performCommand() throws DialogException {
    _display.clear();
    execute();
  }

  /**
   * Executes the command: as defined in the Command pattern, the command's
   * execution should eventually invoke the receiver's action.
   * 
   * @throws DialogException
   *             if something wrong or unexpected occurs.
   */
  protected abstract void execute() throws DialogException;

}
