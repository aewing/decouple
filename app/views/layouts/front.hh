<?hh // decl
class :layouts:front extends :layouts:master {
  public function compose() {
    $this->setTitle('Decouple | Home');
    $this->addStyle('semantic', '//cdnjs.cloudflare.com/ajax/libs/semantic-ui/0.16.1/css/semantic.min.css');
    $this->addStyle('main', '/assets/main.css');
    $this->addScript('semantic', '//cdnjs.cloudflare.com/ajax/libs/semantic-ui/0.16.1/javascript/semantic.min.js');
    return parent::compose();
  }
}
