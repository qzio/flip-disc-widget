
require'flip-disc-widge'

namespace :flipdisc do
  desc "display the next message in the queue"
  task :display_next do
    entry = DisplayQueue.first(:published_at =>  nil, :order => 'created_at')
    if entry
      puts "will display #{entry.str}"
      entry.published_at = Time.now
      entry.save
    else
      puts "nothing in the queue that hasn't been displayed already!'"
    end
  end
end