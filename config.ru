require 'rubygems'
require 'sinatra'
require 'lib/another_logger'
set :env, :production
set :run, false
require 'flip-disc-widget'
if ENV['RAILS_ENV'] == 'production' || ENV['RAILS_ENV'] == 'triage'
  Haml::Template::options[:ugly] = true
end
#use Rack::AnotherLogger
run Sinatra::Application
