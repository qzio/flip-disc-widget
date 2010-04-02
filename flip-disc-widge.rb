require 'rubygems'
require 'sinatra'
require 'lib/simple_helper'

helpers do
  include SimpleHelper
end

get "/" do
  haml :index
end

post "/" do
  @display_str = sanitize params[:str]
  haml :success
end

__END__

@@layout
!!! 5
%html
  %head
    %title flip-disc display widgy!
    %link{:rel=>"stylesheet", :href=>"/stylesheets/reset.css"}
    %link{:rel=>"stylesheet", :href=>"/stylesheets/style.css"}
  %body
    = yield


