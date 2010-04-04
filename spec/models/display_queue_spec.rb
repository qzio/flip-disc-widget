require File.join(File.dirname(__FILE__), '..', 'spec_helper.rb')

describe DisplayQueue do


  describe "validations" do
    it "should not allow empty str" do
      entry = DisplayQueue.new :str => ''
      entry.save.should be_false
      entry.errors.should_not be_empty
      entry.errors.on(:str).should_not be_empty
      DisplayQueue.all.size.should == 0
    end
  end
end
