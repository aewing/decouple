<?hh // decl
class :layouts:master extends :ui:base {
  public string $title = '';
  public Map<string,string> $scripts = Map {};
  public Map<string,string> $styles = Map {};

  public function compose() {
    $head = 
      <head>
          <title>{$this->getTitle()}</title>
      </head>
      ;
    $this->loadScripts($head);
    $this->loadStyles($head);
    return
      <html>
        {$head}
        <body>
          {$this->getChildren()}
        </body>
      </html>
    ;
  }

  public function getTitle() : string {
    return $this->title;
  }

  public function setTitle(string $title) : void {
    $this->title = $title;
  }

  public function addScript($name, $src) : void {
    $this->scripts->set($name, $src);
  }

  public function loadScripts(:xhp $head) : void {
    foreach($this->scripts as $name => $script) {
      $tag = <script type="text/javascript"></script>;
      $tag->setAttribute('src', $script);
      $head->appendChild($tag);
    }
  }

  public function addStyle($name, $src) : void {
    $this->styles->set($name, $src);
  }

  public function loadStyles(:xhp $head) : void {
    foreach($this->styles as $name => $style) {
      $tag = <link rel="stylesheet"/>;
      $tag->setAttribute('href', $style);
      $head->appendChild($tag);
    }
  }
}
