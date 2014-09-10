<?hh // strict
use Decouple\Ui\Ui;
class FrontController {
  public function index() : string {
    $layout = 
      <layouts:master>
        <front:index></front:index>
      </layouts:master>
    ;
    $layout->setTitle('Decouple');
    $layout->addStyle('semantic', '//cdnjs.cloudflare.com/ajax/libs/semantic-ui/0.16.1/css/semantic.min.css');
    $layout->addScript('semantic', '//cdnjs.cloudflare.com/ajax/libs/semantic-ui/0.16.1/javascript/semantic.min.js');
    return (string)$layout;
  }
}
