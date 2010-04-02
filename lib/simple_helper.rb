module SimpleHelper
  def sanitize(str)
    str.gsub(/[^A-Za-z0-9\-\s_åäöÅÄÖ!]/,'_')
  end
  def partial(tpl, options = {})
    tpl_arr = tpl.split "/"
    tpl = tpl_arr[0..-2].join("/")+"_"+tpl_arr[-1]
    options.merge!(:layout => false)
    haml(:"#{tpl}", options)
  end
end
