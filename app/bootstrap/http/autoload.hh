<?hh // partial
set_include_path(get_include_path() . PATH_SEPARATOR . $paths->get('src') . PATH_SEPARATOR . $paths->get('controllers'));
spl_autoload_extensions('.hh,.php');
spl_autoload_register();
$autoloader = new Decouple\Autoload\Autoload($paths);
$autoloader->addNamespace('xhp', $paths->get('views'));
$autoloader->register();
