<?hh // strict
use Decouple\Ui\Ui;
use Decouple\ORM\Connector;
use Decouple\ORM\Query\Raw;
class FrontController {
  public function index(Connector $db, DebugRegistry $debug) : string {
    $view = <front:index></front:index>;
    $news = $db->table('news');
    $query = $news
      ->select(Vector {'id','name','content','author','create_date','image'})
      ->where('delete_date', 'is', new Raw(null))
    ;
    $result = $db->fetchAll($query);

    $content = <div/>;
    if(is_null($result)) {
      $error = 
        <div class="ui error message">
          <h2>Oops!</h2>
          <p>I couldn't find any news articles to display... Try again?</p>
        </div>;
      $content->appendChild($error);
    } else {
      $articles = <news:articles/>;
      foreach($result as $article) {
        $body = <news:article/>;
        $body->setAttribute('title', $article['name']);
        $body->setAttribute('content', $article['content']);
        $body->setAttribute('date', $article['create_date']);
        $body->setAttribute('image', $article['image']);
        $articles->appendChild($body);
      }
      $content->appendChild($articles);
    }
    $view->appendChild($content);
    $view->appendChild(<br/>);
    $view->appendChild(<hr/>);
    $view->appendChild(<br/>);
    $view->appendChild(perf_info($debug));

    return (string)$view;
  }

  public function test() : string {
    return "Test";
  }
}
