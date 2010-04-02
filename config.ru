require 'rubygems'
require 'logger'
require 'sinatra'
require 'lib/another_logger'
set :env, :production
set :run, false
require 'flip-disc-widge'
use Rack::AnotherLogger
run Sinatra::Application
