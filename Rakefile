require'flip-disc-widget'

namespace :flipdisc do
  desc "display the next message in the queue"
  task :display_next do
    entry = DisplayQueue.first(:published_at =>  nil, :order => 'created_at')
    if entry
      #puts "will display #{entry}"
      entry.published_at = Time.now
      system "./clib/print_stuff /dev/ttyUSB0 \"#{entry}\""
      entry.save
    else
      puts "nothing in the queue that hasn't been displayed already!'"
    end
  end
end
