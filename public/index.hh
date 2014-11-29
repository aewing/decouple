<?hh // partial
$start = getrusage();
session_start('decouple');
require_once dirname(__FILE__) . '/../app/bootstrap/http.hh';
