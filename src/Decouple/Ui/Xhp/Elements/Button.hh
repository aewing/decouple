<?hh // strict
final class :ui:button extends :ui:base {
  attribute
    :button,
    enum {'small', 'large'} size = 'small',
    enum {'red', 'blue', 'green'} color = 'red';

  protected function compose() : :xhp {
    $this->addClass($this->getAttribute('size'));
    $this->addClass($this->getAttribute('color'));
    return <button>{$this->getChildren()}</button>;
  }
}
