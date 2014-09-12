<?hh // decl
class :front:index extends :ui:base {
  public function compose() : :xhp {
    return
      <layouts:front>
        <div>
          <h2 class="ui large dividing header">Frontpage</h2>
          {$this->getChildren()}
        </div>
      </layouts:front>
    ;
  }
}
