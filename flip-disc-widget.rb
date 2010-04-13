require 'rubygems'
require 'sinatra'
require 'mongo_mapper'
db_suffix = ''
configure :test do
  db_suffix = '-test'
end
MongoMapper.database = "flip-disc#{db_suffix}"

Dir["lib/*.rb"].each{|f| require "lib/#{File.basename(f)}" }

helpers do
  include SimpleHelper
end

# available resources starts here ---------------

get "/" do
  @entry = DisplayQueue.new :str => 'say something!'
  haml :index
end

post "/" do
  @entry = DisplayQueue.new :str => params[:str]
  if @entry .save
    system "rake flipdisc:display_next >> log/rake.log"
    haml :success
  else
    haml :index
  end
end

get "/msgs" do
  pub_at = (params[:old] && !params[:old].eql?('false')) ? {:$ne => nil} : nil
  @queue_entries = DisplayQueue.all(:published_at => pub_at)
  haml :msgs
end
